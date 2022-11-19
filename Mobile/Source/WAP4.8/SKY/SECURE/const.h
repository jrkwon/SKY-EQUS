#ifndef		_CONST_H
#define _CONST_H

/*	unsigned type declaration */
typedef unsigned long       ul;
typedef unsigned int       	un;
typedef unsigned short      us;
typedef unsigned char       uc;

/* 	success or failure */
#define FAILURE				-1
#define SUCCESS				0

/*	true or false */
#define SSLS_TRUE			1
#define SSLS_FALSE			0

/* 	length of the internal buffer used for handling protocol */
#define LINELEN         	80
#define BYTELEN         	256
#define PAGELEN         	1024

/* 	length of the buffer used for record manipulation */
#define BUFLEN          	(PAGELEN * 3)

/* 	length of the buffer used for handshake message */
#define HANDSHAKE_BUF_SIZE	1024

/*	convenient function */
#define MAXF(a,b)       	(((a) > (b)) ? (a) : (b))
#define MINF(a,b)       	(((a) < (b)) ? (a) : (b))

/* record head length */
#define REC_HEAD_LEN            5

/* record head field position */
#define SEQ_NUM_FIELD           1
#define REC_LEN_FIELD           3

/* bit mask used for record encryption checking */
#define BIT_CIPHER              32
/* bit mask used for record type extraction */
#define REC_TYPE                15

/* record content type */
#define T_CCS                   1
#define T_ALERT                 2
#define T_HANDSHAKE             3
#define T_DATA                  4

/* handshake head length */
#define HANDSHAKE_HEAD_LEN      3

/* protocol version */
#define PROTO_VER               1

/* handshake type */
#define T_CLIENT_HELLO          1
#define T_SERVER_HELLO          2
#define T_KEY_EXCHANGE          3
#define T_FINISHED              20

/* handshake state */
#define ST_NULL                 0
#define ST_WAIT                 1
#define ST_EXKEY                2
#define ST_OPEN                 3

/* alert level */
#define ALRT_WARNING            1
#define ALRT_FATAL              2

/* alert descryption */
#define ALRT_CLOSE_NOTIFY       1
#define ALRT_NO_CONNECTION      2
#define ALRT_UNEXPECTED_MSG     3
#define ALRT_BAD_MAC            4
#define ALRT_DECRYPTION_FAIL    5
#define ALRT_HANDSHAKE_FAIL     6
#define ALRT_ILLEGAL_PRM        7
#define ALRT_DECRYPT_ERR        8
#define ALRT_DUP_FINISHED       9
#define ALRT_PROTO_VER          10
#define ALRT_INTERNAL_ERR       11
#define ALRT_USER_CANCEL        12
#define ALRT_ID_UNREGISTERD     13
#define ALRT_DATA_NOT_ENCRYPTED 14
#define ALRT_INVALID_MSG_LEN    15
#define ALRT_INVALID_CIPHER    	16

/* finished label used for computing verify_data */
#define SFINISH_LABEL           "server finished"
/* length of verify_data */
#define FINISHED_DATA_LEN       12

/* Change Cipher Spec record length */
#define CCS_REC_LEN             6
/* Change Cipher Spec message length */
#define CCS_MSG_LEN             1

/* const related to alert record */
#define ALERT_CHECKSUM_LEN      4
#define ALERT_DATA_LEN          6
#define ALERT_NO_ENC            0
#define ALERT_ENC               1

/* master secret length */
#define MASTER_SEC_LEN          20
/* random length */
#define RANDOM_LEN              16

/* maximum client id length */
#define MAX_CLIENT_ID_SIZE      40

/* MD5 key size */
#define MD5_MAC_KEY_SIZE        16
/* encryption key size */
#define CIPHER_KEY_SIZE         16
/* initialization vector size */
#define CIPHER_IV_SIZE          8

/* MD5 MAC size */
#define MD5_MAC_SIZE            16
/* block length size */
#define CIPHER_BLOCK_SIZE       8
/* number of round */
#define CIPHER_ROUND            16

/* cipher type */
#define T_SKA3					1
#define T_SEED					2

#define SKA3_BLOCK_SIZE			8
#define SEED_BLOCK_SIZE			16

#define PRE_MASTER_LEN			16

#define ELAPSED_SECONDS  315964800

#endif
