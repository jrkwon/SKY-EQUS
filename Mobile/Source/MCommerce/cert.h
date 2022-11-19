//---------------------------------
//
//	cert.h
//
//	by Byung-Jun Yoon
//
//---------------------------------


#ifndef CERT__H
#define CERT__H

#ifdef  __cplusplus
extern "C" {
#endif

#define X509_MAX_SERIAL_LEN				32

typedef struct x509_object{
	int version;
	int serialLen;
	union {
		unsigned int num;
		unsigned char str[X509_MAX_SERIAL_LEN];
	} serial;
	long certOffset;
	long certSize;
	char sig_algo[32];
	char issuer[512];
	char subject[512];
	char notBefore[14];
	char notAfter[14];
	char pubKeyAlgo[32];    
	unsigned char pubKey[256];
	int pubKeyLen;
	unsigned char signature[256];
	int sigLen;
} X509;

int parseDER2X509( unsigned char * der, long length, X509 * x509);

int verify_cert(  unsigned char * user_cert,  long user_cert_len,
                        unsigned char * ca_cert,    long ca_cert_len    );

#ifdef  __cplusplus
}
#endif

#endif	// CERT__H
