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
 * wtlsdef.h
 *
 * Created by Anders Edenbrandt, Thu Jun 01 11:37:33 2000.
 *
 * Revision history:
 *   001012, AED:  Moved some constant declarations to WTLScon.h
 *
 */
#ifndef _wtlsdef_h
#define _wtlsdef_h

#include "tapicmmn.h"

#include "wiptrgt.h"
#include "ml_typw.h"
#include "ctypes.h"
#include "errcodes.h"
#include "wapcvt.h"

#include "pdubuf.h"
#include "buffmgt.h"
#include "glbldecl.h"
#include "wdp_ul.h"

#include "aapicrpt.h"
#include "wtlscon.h"


/*
 * Macron to select the correct MAC algorithm.
 */
#define WTLS_MAC(alg, bearer, n)    WTLS_MAC_ ## alg ## _ ## bearer ## _ ## n


/*
 * The largest protocol version that we support.
 * Currently, 1 is the only possible value.
 */
#define WTLS_PROTOCOL_VERSION   1


/*
 * Return codes
 */
#define RET_OK                      0
#define RET_ERR_FATAL               ALERT_LEVEL_FATAL
#define RET_ERR_CRITICAL            ALERT_LEVEL_CRITICAL
#define RET_ERR_WARNING             ALERT_LEVEL_WARNING
#define RET_FULL                    10
#define RET_ABBR                    11
#define RET_QUEUE_EMPTY             12
#define RET_DUPLICATE_FINISHED      13
#define RET_HELLO_REQUEST           14
#define RET_ALERT                   15
#define RET_VERIFY_CERT             17
#define RET_KEY_EXCH                18
#define RET_EXPORTABLE              19
#define RET_KEY_REFRESH             20
#define RET_ENCRYPT_RECORD          21
#define RET_NEXT_MESSAGE            22
#define RET_DECRYPTION_FAILED       23
#define RET_SEND_MSG                24
#define RET_TIME_REQUIRED           25
#define RET_GET_CERT                26
#define RET_SHARED_SECRET           27
#define RET_USING_CURRENT_SESSION   28
#define RET_NO_PEER                 29


/*
 * The different levels of severity of alert messages.
 * Defined in section 10.2 in the WTLS specification.
 */
#define ALERT_LEVEL_WARNING   1
#define ALERT_LEVEL_CRITICAL  2
#define ALERT_LEVEL_FATAL     3

/*
 * All the different descriptions of alert messages.
 * Defined in section 10.2 in the WTLS specification.
 */
#define ALERT_DESC_CONNECTION_CLOSE_NOTIFY       0
#define ALERT_DESC_SESSION_CLOSE_NOTIFY          1
#define ALERT_DESC_NO_CONNECTION                 5
#define ALERT_DESC_UNEXPECTED_MESSAGE           10
#define ALERT_DESC_TIME_REQUIRED                11
#define ALERT_DESC_BAD_RECORD_MAC               20
#define ALERT_DESC_DECRYPTION_FAILED            21
#define ALERT_DESC_RECORD_OVERFLOW              22
#define ALERT_DESC_DECOMPRESSION_FAILURE        30
#define ALERT_DESC_HANDSHAKE_FAILURE            40
#define ALERT_DESC_BAD_CERTIFICATE              42
#define ALERT_DESC_UNSUPPORTED_CERTIFICATE      43
#define ALERT_DESC_CERTIFICATE_REVOKED          44
#define ALERT_DESC_CERTIFICATE_EXPIRED          45
#define ALERT_DESC_CERTIFICATE_UNKNOWN          46
#define ALERT_DESC_ILLEGAL_PARAMETER            47
#define ALERT_DESC_UNKNOWN_CA                   48
#define ALERT_DESC_ACCESS_DENIED                49
#define ALERT_DESC_DECODE_ERROR                 50
#define ALERT_DESC_DECRYPT_ERROR                51
#define ALERT_DESC_UNKNOWN_KEY_ID               52
#define ALERT_DESC_DISABLED_KEY_ID              53
#define ALERT_DESC_KEY_EXCHANGE_DISABLED        54
#define ALERT_DESC_SESSION_NOT_READY            55
#define ALERT_DESC_UNKNOWN_PARAMETER_INDEX      56
#define ALERT_DESC_DUPLICATE_FINISHED_RECEIVED  57
#define ALERT_DESC_EXPORT_RESTRICTION           60
#define ALERT_DESC_PROTOCOL_VERSION             70
#define ALERT_DESC_INSUFFICIENT_SECURITY        71
#define ALERT_DESC_INTERNAL_ERROR               80
#define ALERT_DESC_USER_CANCELED                90
#define ALERT_DESC_NO_RENEGOTIATION            100

/*
 * The Alert messsage data type.
 * Defined in section 10.2 in the WTLS specification.
 */ 
typedef struct {
  UINT8  level;
  UINT8  description;
  UINT32 checksum;
} wtls_alert_t;


typedef struct {
  UINT8 length;
  BYTE  id[8];
} wtls_session_id;


typedef BYTE wtls_random_t[16];

/*
 * The Record data structure.
 */
#define CONTENT_TYPE_EMPTY           0
#define CONTENT_TYPE_CCS             1
#define CONTENT_TYPE_ALERT           2
#define CONTENT_TYPE_HANDSHAKE       3
#define CONTENT_TYPE_DATA            4
#define CONTENT_TYPE_HELLO_REQUEST  10
#define CONTENT_TYPE_ERROR          15

/* The bit fields in the record type byte: */
#define RECTYPE_CONTENT_TYPE  0x0f
#define RECTYPE_USE_CS        0x20
#define RECTYPE_SEQNUM        0x40
#define RECTYPE_LENGTH_FIELD  0x80

typedef struct wtls_rec_st {
  struct wtls_rec_st *next;     /* Pointer to the next record */

  UINT8 is_first;   /* This is the first record in a SDU. */
  UINT8  rec_type;  /* Type byte, sent first in a record. */
  UINT16 seqnum;    /* Sequence number, if in use. */
  UINT16 length;    /* Length of fragment */
  BYTE   *fragment; /* Data part */
} wtls_record_t;


#define IDENTIFIER_NULL           0
#define IDENTIFIER_TEXT           1
#define IDENTIFIER_BINARY         2
#define IDENTIFIER_KEY_HASH_SHA 254
#define IDENTIFIER_X509_NAME    255

typedef struct {
  UINT8 identifier_type;
  union {
    struct {
      UINT16 character_set;
      UINT8  namelen;
      BYTE   *name;
    } text;
    struct {
      UINT8 idlen;
      BYTE  *identifier;
    } binary;
    struct {
      BYTE *key_hash;
    } key_hash_SHA;
    struct {
      UINT8 namelen;
      BYTE  *distinguished_name;
    } x509_name;
  } _u;
} wtls_identifier_t;


typedef UINT8 wtls_MAC_algorithm;

/* The Compression algorithms defined by WTLS. */
#define COMPRESS_NULL           0

/* Bulk encryption algorithms are either of the stream type, or of
 * the block type. */
#define CIPHER_TYPE_STREAM   1
#define CIPHER_TYPE_BLOCK    2

typedef struct {
  BulkCipherAlgorithm bulk_cipher_alg;
  wtls_MAC_algorithm  mac_alg;
} wtls_cipher_suite;

typedef struct {
  KeyExchangeSuite   key_exchange_suite;
  ParameterSpecifier parameter_specifier;
  wtls_identifier_t  identifier;
} wtls_key_exchange_id;

/*
 * Handshake message types.
 */
#define HANDSHK_HELLO_REQUEST           0
#define HANDSHK_CLIENT_HELLO            1
#define HANDSHK_SERVER_HELLO            2
#define HANDSHK_CERTIFICATE            11
#define HANDSHK_SERVER_KEY_EXCHANGE    12
#define HANDSHK_CERTIFICATE_REQUEST    13
#define HANDSHK_SERVER_HELLO_DONE      14
#define HANDSHK_CERTIFICATE_VERIFY     15
#define HANDSHK_CLIENT_KEY_EXCHANGE    16
#define HANDSHK_FINISHED               20
#define HANDSHK_ERROR                  90
#define HANDSHK_EMPTY                  99

typedef UINT8 wtls_handshake_t;

/*
 * All possible handshake messages, internal representation.
 */
typedef struct {
  wtls_handshake_t msg_type;
  UINT16           length;
} wtls_hello_request, wtls_server_hello_done;

typedef struct {
  wtls_handshake_t msg_type;
  UINT16           length;
  UINT8            client_version;
  wtls_random_t    random;
  wtls_session_id  session_id;
  UINT16           client_key_id_len;
  BYTE             *client_key_ids;
  UINT16           trusted_key_id_len;
  BYTE             *trusted_key_ids;
  UINT8            cipher_suites_len;
  BYTE             *cipher_suites;
  UINT8            num_compression_methods;
  UINT8            *compression_methods;
  UINT8            seqnum_mode;
  UINT8            key_refresh_rate;
} wtls_client_hello;

typedef struct {
  wtls_handshake_t  msg_type;
  UINT16            length;
  UINT8             server_version;
  wtls_random_t     random;
  wtls_session_id   session_id;
  UINT8             client_key_id;
  wtls_cipher_suite cipher_suite;
  UINT8             compression_method;
  UINT8             seqnum_mode;
  UINT8             key_refresh_rate;
} wtls_server_hello;

typedef struct {
  wtls_handshake_t msg_type;
  UINT16           length;
  UINT16           buflen;
  BYTE             *buf;
} wtls_server_certificate, wtls_client_certificate;

typedef struct {
  wtls_handshake_t   msg_type;
  UINT16             length;
  ParameterSpecifier parameter_specifier;
  PublicKey          public_key;
} wtls_server_key_exchange;

typedef struct {
  wtls_handshake_t msg_type;
  UINT16           length;
  UINT16           buflen;
  BYTE             *buf;
} wtls_certificate_request;

typedef struct {
  wtls_handshake_t msg_type;
  UINT16           length;
  union {
    UINT16         len16;
    UINT8          len8;
  } buflen;
  BYTE             *buf;
} wtls_client_key_exchange;

typedef struct {
  wtls_handshake_t msg_type;
  UINT16           length;
  UINT16           siglen;
  BYTE             *signature;
} wtls_certificate_verify;

typedef struct {
  wtls_handshake_t msg_type;
  UINT16           length;
  BYTE             verify_data[12];
} wtls_finished;

typedef struct {
  wtls_session_id session_id;
  UINT8         bulk_cipher_alg;
  UINT8         mac_alg;
  UINT8         hash_alg;
  UINT8         compression_alg;
  wtls_random_t client_hello_random;
  wtls_random_t server_hello_random;
  UINT8         key_refresh_rate;
  UINT8         seqnum_mode;
  UINT8         master_secret_id;
} wtls_pending_state;


#define MAX_NUM_HANDSHAKE_MESSAGES      11

typedef struct {
  BYTE   *buf;
  UINT16 buflen;
} wtls_handshake_msg;

#define BIT_CLIENT_HELLO                   0x1
#define BIT_SERVER_HELLO                   0x2
#define BIT_SERVER_CERTIFICATE             0x4
#define BIT_SERVER_KEY_EXCHANGE            0x8
#define BIT_SERVER_CERTIFICATE_REQUEST    0x10
#define BIT_SERVER_HELLO_DONE             0x20
#define BIT_CLIENT_CERTIFICATE            0x40
#define BIT_CLIENT_KEY_EXCHANGE           0x80
#define BIT_CLIENT_CERTIFICATE_VERIFY    0x100
#define BIT_CLIENT_FINISHED              0x200
#define BIT_SERVER_FINISHED              0x400

typedef struct {
  wtls_pending_state pending;  /* The "pending" connection state */

  wtls_key_exchange_id key_exch;  /* Selected key exchange method */
  UINT8                need_client_key_exchange;
  BYTE                 *key_id;
  UINT16               key_idlen;
  UINT8  abortable; /* Can this handshake be aborted, and the
                     * connection state be restored? */

  /* All the messages that make up the handshake.
   * Some of these will be empty */
  wtls_client_hello        client_hello;
  wtls_server_hello        server_hello;
  wtls_server_certificate  server_certificate;
  wtls_server_key_exchange server_key_exchange;
  wtls_certificate_request certificate_request;
  wtls_server_hello_done   server_hello_done;
  wtls_client_certificate  client_certificate;
  wtls_client_key_exchange client_key_exchange;
  wtls_certificate_verify  certificate_verify;
  wtls_finished            client_finished;
  wtls_finished            server_finished;

  /* To keep track of the order in which the handshake messages
   * have appeared. */
  wtls_handshake_msg       hm[MAX_NUM_HANDSHAKE_MESSAGES];
  UINT8                    num_handshake_msgs;
  UINT32                   msgs;
} wtls_handshake_state;


typedef struct {
  /* Bulk encryption */
  BulkCipherAlgorithm bulk_cipher_alg;
  UINT8               is_exportable;
  UINT8               type;
  UINT8               key_material_length;
  UINT8               key_size;
  UINT8               iv_size;
  UINT8               block_size;

  /* Secure hash */
  wtls_MAC_algorithm  mac_alg;
  UINT8               mac_key_size;
  UINT8               mac_size;
  UINT8               full_mac_size;
  UINT8               mac_block_size;
  HashHandle          hash_handle;
  HashAlgorithm       hash_alg;

  /* Compression */
  UINT8               compression_alg;
} wtls_crypto_t;

#define CONSTATE_READ            1
#define CONSTATE_WRITE           2

/* Sequence number mode: whether we use sequence numbers or not. */
#define SEQNUMMODE_OFF          0
#define SEQNUMMODE_IMPLICIT     1
#define SEQNUMMODE_EXPLICIT     2

#define MAX_SEQNUM             0xffffU  /* The largest sequence number,
                                         * being 2^16 - 1. */
/* The largest values for some parameters. Convenient if you want to
 * declare static arrays for some of the cryptographic operations.
 * All sizes are in bytes. */
#define MAX_KEY_SIZE            24      /* Max length of encryption keys */
#define MAX_IV_SIZE              8
#define MAX_HASH_MAC_SIZE       20
#define MAX_HASH_KEY_SIZE       20
#define MAX_HASH_BLOCK_SIZE     64

typedef struct {
  wtls_session_id session_id;
  BYTE          mac_secret[MAX_HASH_KEY_SIZE];
  BYTE          encryption_key[MAX_KEY_SIZE];
  BYTE          iv[MAX_IV_SIZE];
  wtls_random_t client_hello_random;
  wtls_random_t server_hello_random;
  UINT16        seqnum;             /* Sequence number. */
  UINT8         key_refresh_rate;   /* Key refresh rate */
  UINT16        last_refresh;       /* Last sequence number used for key */
  UINT8         use_cipher_spec;    /* Are we using Cipher Spec != NULL? */
  wtls_crypto_t cobj;               /* Cryptographic state. */
  UINT8         seqnum_mode;
  UINT8         master_secret_id;
} wtls_connection_state;

/*
 * This is the wtls_connection_t data structure.
 * Each active instance of the WTLS_CM SDL process
 * has one copy of a record of this type.
 */
typedef struct wtls_connection_st {
  AddressType client_addr;         /* The address quadruple. */
  AddressType server_addr;

  SDL_PId     cm_proc;             /* The Connection Manager SDL process for
                                   * this connection */
  SDL_PId     rec_proc;

  SDL_Natural path;

  wtls_handshake_state *h_state;  /* Parameters used during the handshake
                                   * phase. The "pending state" is kept
                                   * here. */

  wtls_connection_state read;     /* The "current states", one for reading */
  wtls_connection_state write;    /* and one for writing. */

  UINT32      read_cksum;         /* Alert checksum of last record recv'd */
  UINT32      write_cksum[4];     /* Alert checksum of last 4 records sent */
  UINT16      num_cksums;         /* Number of write-checksums stored */

  UINT32      mask;               /* Mask indicating which sequence numbers *
                                   * we have seen (window of size 32). */

  wtls_record_t *out_records;     /* Records waiting to be sent. */
  char          *proxy_provider_id;
  int            allow_anon;
} wtls_connection_t;



/*
 * Conversion functions from Convert.c
 */
INT16
wtls_cvt_alert (wap_cvt_t *obj, wtls_alert_t *p);

INT16
wtls_cvt_identifier (wap_cvt_t *obj, wtls_identifier_t *p);

INT16
wtls_cvt_parameter_specifier (wap_cvt_t *obj, ParameterSpecifier *p);

INT16
wtls_cvt_public_key (wap_cvt_t *obj,
                     KeyExchangeSuite kes, PublicKey *p);

INT16
wtls_cvt_key_exchange_id (wap_cvt_t *obj, wtls_key_exchange_id *p);

INT16
wtls_cvt_client_hello (wap_cvt_t *obj, wtls_client_hello *p);

INT16
wtls_cvt_server_hello (wap_cvt_t *obj, wtls_server_hello *p);

INT16
wtls_cvt_server_certificate (wap_cvt_t *obj,
                             wtls_server_certificate *p);

INT16
wtls_cvt_server_key_exchange (wap_cvt_t *obj, KeyExchangeSuite kes,
                              wtls_server_key_exchange *p);

INT16
wtls_cvt_certificate_request (wap_cvt_t *obj,
                              wtls_certificate_request *p);

INT16
wtls_cvt_server_hello_done (wap_cvt_t *obj, wtls_server_hello_done *p);

INT16
wtls_cvt_client_certificate (wap_cvt_t *obj, wtls_client_certificate *p);

INT16
wtls_cvt_client_key_exchange (wap_cvt_t *obj, KeyExchangeSuite kes,
                              wtls_client_key_exchange *p);

INT16
wtls_cvt_certificate_verify (wap_cvt_t *obj, wtls_certificate_verify *p);

INT16
wtls_cvt_finished (wap_cvt_t *obj, wtls_finished *p);

INT16
wtls_cvt_record (wap_cvt_t *obj, wtls_record_t *p);


/*
 * Functions from Alert.c
 */
UINT32
wtls_alert_compute_checksum (BYTE *msg, UINT16 msglen);


/*
 * Functions from Connectn.c
 */
void
wtls_connection_free (void *connptr);

INT16
wtls_connection_state_init (wtls_connection_state *cstate);

INT16
wtls_pending_state_init (wtls_pending_state *pstate);

INT16
wtls_pending_state_install (wtls_connection_state *cstate,
                            wtls_pending_state *pstate);


/*
 * Functions from ConnMgr.c
 */
void *
wtls_cm_init (SDL_PId cm_proc, SDL_PId rec_proc,
              AddressType src_addr, AddressType dst_addr,
              void *proxy_provider_id, SDL_Boolean allow_anon);

void
wtls_cm_terminate (void *connptr, SDL_Boolean established);

void
wtls_cm_get_connection_parameters (SDL_Octet bearer,
                                   SDL_Integer *maxRetransmissions,
                                   SDL_Integer *retryInterval);
SDL_Integer
wtls_cm_handshake_init (void *connptr, SDL_Boolean established);

INT16
wtls_cm_session_fetch1 (void *connptr, SDL_Integer result,
                        SDL_Octet session_options,
                        void *session_id, SDL_Octet session_length,
                        SDL_Octet mac_alg, SDL_Octet cipher_alg,
                        SDL_Octet compression_alg, void *private_key_id,
                        SDL_Natural creation_time);

INT16
wtls_cm_session_fetch2 (void *connptr, SDL_Integer result,
                        SDL_Octet master_secret_id);

INT16
wtls_cm_session_fetch3 (void *connptr, SDL_Integer result,
                        SDL_Octet session_options,
                        void *session_id, SDL_Octet session_length,
                        SDL_Octet mac_alg, SDL_Octet cipher_alg,
                        SDL_Octet compression_alg, void *private_key_id,
                        SDL_Natural creation_time);

SDL_Integer
wtls_cm_create_client_hello (void *connptr);

SDL_Integer
wtls_cm_continue_create_client_hello (void *connptr, SDL_Integer result,
                                      void *cmptr, SDL_Natural cmlen,
                                      void *kxptr, SDL_Natural kxlen,
                                      void *tkptr, SDL_Natural tklen,
                                      void **pbufptr, SDL_Natural *pbuflen);

SDL_Integer
wtls_cm_process_server_hello (void *connptr, void *bufptr, SDL_Natural buflen);

SDL_Integer
wtls_cm_process_server_certificate (void *connptr,
                                    void *bufptr, SDL_Natural buflen);

SDL_Integer
wtls_cm_process_server_key_exchange (void *connptr,
                                     void *bufptr, SDL_Natural buflen);

SDL_Integer
wtls_cm_process_certificate_request (void *connptr,
                                     void *bufptr, SDL_Natural buflen);

INT16
wtls_cm_process_server_hello_done (void *connptr,
                                   void *bufptr, SDL_Natural buflen);

SDL_Integer
wtls_cm_create_client_certificate_msg (void *connptr);

SDL_Integer
wtls_cm_continue_create_client_certificate_msg (void *connptr);

SDL_Integer
wtls_cm_process_client_certificate_response (void *connptr,
                                             SDL_Natural result,
                                             void *key_id,
                                             SDL_Natural key_idlen,
                                             void **pbufptr,
                                             SDL_Natural *pbuflen);
SDL_Integer
wtls_cm_create_client_key_exchange (void *connptr,
                                    void **pbufptr, SDL_Natural *pbuflen);

SDL_Integer
wtls_cm_create_certificate_verify_msg (void *connptr);

SDL_Integer
wtls_cm_process_compute_signature_response (void *connptr,
                                            SDL_Natural result,
                                            void **pbufptr,
                                            SDL_Natural *pbuflen);
SDL_Integer
wtls_cm_verify_cert (void *connptr);

SDL_Integer
wtls_cm_process_verify_cert_result (SDL_Integer res);

SDL_Integer
wtls_cm_create_verify_data (void *connptr);

SDL_Integer
wtls_cm_create_finished (void *connptr, void **pbufptr, SDL_Natural *pbuflen);

SDL_Integer
wtls_cm_process_finished (void *connptr, void *bufptr, SDL_Natural buflen);

SDL_Integer
wtls_cm_check_server_finished (void *connptr, SDL_Integer result,
                               void *bufptr, SDL_Natural buflen);

SDL_Integer
wtls_cm_get_handshake_type (void *bufptr, SDL_Natural buflen);

SDL_Integer
wtls_cm_process_data_msg (void *connptr,
                          void *bufptr, SDL_Natural buflen,
                          TDUnitdataIndType *data_ind);

SDL_Integer
wtls_cm_opening_msg_ind (void *connptr, SDL_Natural msg_type,
                         void *bufptr, SDL_Natural buflen,
                         TDUnitdataIndType *data_ind);

SDL_Integer
wtls_cm_get_next_outbound_message (void *connptr,
                                   void **precptr);

void
wtls_cm_handshake_finished (void *connptr);

void
wtls_cm_delete_connections_session (void *connptr);

SDL_Boolean
wtls_cm_can_abort_handshake (void *connptr);

void
wtls_cm_abort_handshake (void *connptr);

/*
 * Functions from Crypto.c
 */
INT16
wtls_crypto_init (wtls_crypto_t *cobj,
                  BulkCipherAlgorithm bulk_cipher_alg,
                  wtls_MAC_algorithm mac_alg,
                  UINT8 compression_alg);

HashAlgorithm
wtls_crypto_MAC_to_hash_alg (wtls_MAC_algorithm alg);

INT16
wtls_crypto_encrypt (wtls_crypto_t *cobj,
                     BYTE *key, BYTE *iv,
                     BYTE *buf, UINT16 buflen,
                     BYTE *outbuf);

INT16
wtls_crypto_decrypt (wtls_crypto_t *cobj,
                     BYTE *key, BYTE *iv,
                     BYTE *buf, UINT16 buflen,
                     BYTE *outbuf);
INT16
wtls_crypto_hash (wtls_crypto_t *cobj, BYTE *buf, UINT16 buflen, BYTE *digest);

INT16
wtls_crypto_hash_init (wtls_crypto_t *cobj);

INT16
wtls_crypto_hash_update (wtls_crypto_t *cobj, BYTE *buf, UINT16 buflen);

INT16
wtls_crypto_hash_final (wtls_crypto_t *cobj, BYTE *digest);

INT16
wtls_crypto_HMAC_init (wtls_crypto_t *cobj, BYTE *key, UINT16 keylen);

INT16
wtls_crypto_HMAC_update (wtls_crypto_t *cobj, BYTE *data, UINT16 datalen);

INT16
wtls_crypto_HMAC_final (wtls_crypto_t *cobj,
                        BYTE *key, UINT16 keylen, BYTE *result);

INT16
wtls_crypto_MAC (wtls_crypto_t *cobj, BYTE *mac_secret,
                 BYTE *buf, UINT16 buflen,
                 UINT16 seqnum, UINT8 rec_type,
                 BYTE *mac);

INT16
wtls_crypto_key_refresh1 (void *connptr, SDL_Natural state);

INT16
wtls_crypto_key_refresh2 (void *connptr, SDL_Natural state,
                          void *buf, SDL_Natural buflen);

INT16
wtls_crypto_key_refresh3 (void *connptr, SDL_Natural state);

INT16
wtls_crypto_key_refresh4 (void *connptr, SDL_Natural state, void *buf,
                          SDL_Natural buflen);

INT16
wtls_crypto_key_refresh5 (void *connptr, SDL_Natural state,
                          void *buf, SDL_Natural buflen);


/*
 * Functions from Handshake.c
 */
wtls_handshake_state *
wtls_handshake_state_new (void);

INT16
wtls_handshake_state_free (wtls_handshake_state *hs);

INT16
wtls_handshake_get_msg_type (BYTE *buf, UINT16 buflen);


/*
 * Functions from KeyExchange.c
 */
SDL_Integer
wtls_key_exchange_init (void *connptr);

SDL_Integer
wtls_key_exchange_continue (void *connptr,
                            SDL_Integer result, SDL_Octet master_secret_id,
                            void *public_value,
                            SDL_Natural public_valuelen);


/*
 * Functions from MgtEnt.c
 */
void
wtls_me_init (void);

void
wtls_me_terminate (void);


/*
 * Functions from Record.c
 */
void
wtls_rec_init (void);

INT16
wtls_rec_encrypt_record (void *connptr, void *recptr);

INT16
wtls_rec_decrypt_record (wtls_connection_state *cstate, wtls_record_t *rec);

INT16
wtls_rec_create_SDU (void *connptr, void *recptr, TDUnitdataReqType *data_req);

void
wtls_rec_append_record (void *connptr, void *recptr);

SDL_Integer
wtls_rec_create_handshake_record (void *connptr,
                                  void *buf, SDL_Natural buflen,
                                  void **pprec);

INT16
wtls_rec_create_CCS_record (void *connptr, void **pprec);

INT16
wtls_rec_create_alert_record (void *connptr,
                              SDL_Integer alert_level, SDL_Integer alert_desc,
                              void **pprec);

SDL_Integer
wtls_rec_prepare_data_record (void *connptr, void *recptr);

SDL_Integer
wtls_rec_create_data_and_buffer_SDU (void *connptr,
                                     void *recptr,
                                     TDUnitdataReqType *data_req);

INT16
wtls_rec_create_buffer_SDU (void *connptr, TDUnitdataReqType *data_req);

void
wtls_rec_clear_buffer (void *connptr);

void
wtls_rec_list_append (wtls_record_t *rec, wtls_record_t **rec_list);

void
wtls_rec_list_delete (wtls_record_t **rlist);

wtls_record_t *
wtls_rec_list_pop (wtls_record_t **rec_list);

void
wtls_rec_delete_record (wtls_record_t *rec);

INT16
wtls_rec_next_msg (void *connptr, void **pprec, SDL_Boolean same_SDU);

INT16
wtls_rec_continue_next_msg (void *connptr, void *recptr,
                            SDL_Natural *pmsg_type,
                            void **pbuf, SDL_Natural *pbuflen,
                            SDL_Natural *alert_level,
                            SDL_Natural *alert_desc);

INT16
wtls_rec_install_pending_read (void *connptr);

INT16
wtls_rec_install_pending_write (void *connptr);


/*
 * Functions from WTLSMain.c
 */
void
wtls_main_init (void);

void
wtls_main_terminate (void);

SDL_Integer
wtls_main_new_connection (SDL_PId cm_proc, SDL_PId rec_proc,
                          AddressType *src_addr, AddressType *dst_addr,
                          SDL_Integer handle, SDL_Boolean needs_confirmation);

SDL_Integer
wtls_main_delete_connection (SDL_PId cm_proc);

SDL_PId
wtls_main_pop_connection (void);

SDL_PId
wtls_main_find_connection (AddressType *src_addr, AddressType *dst_addr,
                           SDL_PId *rec_proc);

SDL_PId
wtls_main_find_connection_by_port (SDL_Natural portnum);

SDL_Integer
wtls_main_append_message (SDL_PId cm_proc, pdubuf *msg);

SDL_Integer
wtls_main_check_pid (SDL_PId pid);

INT16
wtls_main_is_cleartext_alert (pdubuf *data);

SDL_Integer
wtls_main_create_no_connection_alert (TDUnitdataIndType *dataInd,
                                      TDUnitdataReqType *dataReq);

void
wtls_main_set_connection_pointer (SDL_PId pid, void *connptr);

void *
wtls_main_get_connection_pointer (SDL_PId pid);

wtls_record_t *
wtls_main_pop_incoming_message (SDL_PId pid);

wtls_record_t *
wtls_main_peek_incoming_message (SDL_PId pid);

wtls_record_t *
wtls_main_pop_outbound_message (SDL_PId pid);

INT16
wtls_main_clear_message_queue (SDL_PId pid);

SDL_Integer
wtls_main_split_SDU (SDL_PId cm_proc, pdubuf *SDU);

SDL_Integer
wtls_main_check_hello_request (pdubuf *pdu);

void
wtls_main_set_close_on_done (SDL_PId cm_proc,
                             SDL_Natural portnum,
                             SDL_Integer path);

SDL_Boolean
wtls_main_get_close_on_done (SDL_PId cm_proc,
                             SDL_Natural *portnum,
                             SDL_Integer *path);

SDL_Boolean
wtls_main_needs_confirmation (SDL_PId cm_proc,
                              SDL_Integer *handle);

/*
 * Functions from WTLSerr.c
 */
void
wtls_err_set (SDL_Integer errtype, SDL_Integer errcode,
              SDL_Integer sendAlert,
              SDL_Integer alertLevel, SDL_Integer alertDesc);

void
wtls_err_get (SDL_Integer *errtype, SDL_Integer *errcode,
              SDL_Integer *sendAlert,
              SDL_Integer *alertLevel, SDL_Integer *alertDesc);

void
wtls_err_check (SDL_Integer *errtype, SDL_Integer *errcode,
                SDL_Integer *sendAlert,
                SDL_Integer *alertLevel, SDL_Integer *alertDesc);

void
wtls_err_clear (void);

#ifdef LOG_WTLS
#include "logging.h"
#endif

#endif
