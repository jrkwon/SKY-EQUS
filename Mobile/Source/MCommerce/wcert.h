//------------------------------------
//
//	wcert.h
//
//	 - Parse WTLS Certificate
//
//	2000. 9. by Byung-Jun Yoon
//
//	edit: 2001.06.23	Kim Jae Hyun
//
//------------------------------------

#ifndef WCERT__H
#define WCERT__H

#include "rsa.h"

#ifdef __cplusplus
extern "C" {
#endif


enum {	ID_SIGALGO		= 0,
		ID_PUBKEYALGO	= 1 };

// Identifier Type

enum {	ID_TYPE_NULL            = 0,
		ID_TYPE_TEXT            = 1,
		ID_TYPE_BINARY          = 2,
		ID_TYPE_KEY_HASH_SHA    = 254,
		ID_TYPE_X509_NAME       = 255 };

// SignatureAlgorithm

enum {	ID_SIGALGO_ANONYMOUS    = 0,
		ID_SIGALGO_ECDSA_SHA    = 1,
		ID_SIGALGO_RSA_SHA      = 2 };

// PublicKeyType

enum {	ID_PUBKEY_TYPE_RSA      = 2,
		ID_PUBKEY_TYPE_ECDH     = 3,
		ID_PUBKEY_TYPE_ECDSA    = 4 };




typedef struct wtls_certificate {
	int version;
	long certSize;
	int sigAlgo;
	unsigned char issuerCharSet[2];
	unsigned char subjectCharSet[2];
	unsigned long notBefore;
	unsigned long notAfter;
	char issuer[512];
	char subject[512];
	int pubKeyAlgo;

	int rsaExpLen;
	unsigned char rsaExponent[16];
	int rsaModLen;
	unsigned char rsaModulus[256];

	unsigned char signature[256];
	int sigLen;
} WTLSCert;


int parseWTLSCert( unsigned char * cert, long length, WTLSCert * wCert);

void PrintWTLSCert( WTLSCert * wCert );

RSA * getRSAPubKey( WTLSCert * wCert );

int verifyWTLSCert( unsigned char * cert, long certLen, RSA * caPubKey);

//int verifyCert( int certFmt, unsigned char * userCert, long userCertLen,
	   	//unsigned char * keyID );
int verifyCert( int certFmt, unsigned char * userCert, long userCertLen );

unsigned char *getCACertificate( int certFmt, const unsigned char * keyID, int * nLength );

//int verifyWTLSCertByCACert( unsigned char * userCert, long userCertLen,
							//unsigned char * caCert, long caCertLen );

#ifdef __cplusplus
}
#endif

#endif	// WCERT__H

