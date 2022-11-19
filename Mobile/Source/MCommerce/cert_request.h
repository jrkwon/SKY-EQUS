//------------------------------------
//
//	cert_request.h
//
//	create certificate request
//
//	2000. 12. by Byung-Jun Yoon
//
//------------------------------------

#ifndef CERT_REQUEST__H
#define CERT_REQUEST__H

// added 2001.08.29
#ifndef __WTLS__KEY_USAGE__
#define __WTLS__KEY_USAGE__
#define V_WTLS__DIGITAL_SIGNATURE   0
#define V_WTLS__KEY_AGREEMENT       1
#endif /* __WTLS__KEY_USAGE__ */

enum {	CMD_CERT_REQUEST	= 1,
		CMD_CERT_REVOKE		= 2 };


unsigned char * certRequest( int nCommand, const char * subject,
							 const char * policy, int * reqLength);


typedef struct __cert_request {
	int type;
	unsigned char charSet[2];
	char * subject;
	union {
		int algo;
		int reason;
	} code;
	unsigned char * pubkey;
	int pubkey_len;
	unsigned char * param;
	int param_len;
} CERT_REQUEST;


CERT_REQUEST * acceptCertRequest( unsigned char * certReq, int nLength);

#endif // CERT_REQUEST__H

