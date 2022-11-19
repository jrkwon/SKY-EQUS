#ifndef WAP_MIME__H
#define WAP_MIME__H


//----------------------------------------
//
//	wap_mime.h
//
//	Create/Process MIME Contens
//
//	2001. 6. by Byung-Han Hwang
//
// Supported MIME Contents
//   1. application/vnd.wap.cert-response
//   2. application/vnd.wap.hashed-certificate
//   3. application/vnd.wap.signed-certificate
//   주) 2,3은 현재 WTLS CA Cert만 지원
//----------------------------------------

//------------------------------------------------------
//
//	MIME Content :
//		application/vnd.wap.hashed-certificate
//
//	certResponse.h
//
//	Create/Process MIME Contens
//
//	2001. 1. by Byung-Jun Yoon
//
//------------------------------------------------------

// CertRespType

/* enum { cert_info(0), cert(1), referral(2), (255) } 
   CertRespType; */
#ifndef __WTLS__CERT_RESPONSE_TYPE__
#define __WTLS__CERT_RESPONSE_TYPE__
#define V_WTLS__CERT_INFO						0
#define V_WTLS__CERT							1
#define V_WTLS__REFERRAL						2
#endif /* __WTLS__CERT_RESPONSE_TYPE__ */

typedef struct _cert_response {
	char * name;
	unsigned char akid[20];		// authority(issuer) key identifier
	unsigned char skid[20];		// subject key identifier
	unsigned char * cert;
	int cert_len;
	int type;					// CertRespType
} CERT_RESPONSE;

unsigned char * createCertResponse(	const char * strCertDisplayName,
									const unsigned char * issuerKeyId,
									const unsigned char * subjectKeyId,
									const unsigned char * cert,
									int nCertLen, int nCertRespType,
									int * length, int *errMsg );

CERT_RESPONSE * 
acceptCertResponse( const unsigned char * certResponse,
									int length, int *errMsg );

CERT_RESPONSE * new_cert_response(void);

void free_cert_response( CERT_RESPONSE * response);


//------------------------------------------------------
//
//	MIME Content :
//		application/vnd.wap.hashed-certificate
//
//	- create hashed certificate
//	- parse hashed certificate
//	- create and verify out of band hash
//
//	2001. 5. by Ji-Hyun Sohn
//
//------------------------------------------------------

#define DECIMAL_DIGIT_OUTOFBANDHASH 30
#define SHA_DIGEST_LENGTH 20

#define WRITE_TO_BUF( target, source, length, offset ) \
        { memcpy(&target[offset], source, length); offset += length; }

#define READ_FROM_BUF( source, target, length, offset ) \
	    { memcpy(target, &source[offset], length); offset += length; }

/* enum { anonymous(0), ecdsa_sha(1), rsa_sha(2), (255) }
   SignatureAlgorithm; */
#ifndef __WTLS__SIGNATURE_ALGORITHM__
#define __WTLS__SIGNATURE_ALGORITHM__
#define V_WTLS__ANONYMOUS	0
#define V_WTLS__ECDSA_SHA	1
#define V_WTLS__RSA_SHA		2
#endif /* __WTLS__SIGNATURE_ALGORITM__ */

// Hash Algorithm
enum {
    eSHA1    = 0
};

/* enum { WTLSCert(1), X509Cert(2), X968Cert(3), CertURL(4), (255) }
   CertificateFormat; */
#ifndef __WTLS__CERTIFICATE_FROMAT__
#define __WTLS__CERTIFICATE_FROMAT__
#define V_WTLS__WTLS_CERT					1
#define V_WTLS__X509_CERT					2
#define V_WTLS__X968_CERT					3
#define V_WTLS__CERT_URL					4
#endif /* __WTLS__CERTIFICATE_FROMAT__ */

typedef struct _TBHTrustedCAInfo {
    int version;
    unsigned char charSet[2];
    char *displayName;
    int trustedCACertFormat;
    unsigned char *trustedCACert;
	// added
	int trustedCACert_len;
    char *cainfo_url;
    int hash_alg;
} TBHTrustedCAInfo;

unsigned char *createHashedCertificate( int version, int charSet,
										char *displayName,
										int trustedCACertFormat,
										unsigned char *cert,
										int certLen, char *cainfo_url,
										int hash_alg, int *length,
										int *errMsg );

TBHTrustedCAInfo *parseHashedCertificate(	unsigned char *hashedCert, 
											int hashedCertLen,
											int *errMsg );

//swjung 
int verifyHashedCertificate( unsigned char *hashedCert, int hashedCertLen, char *strOutOfBandHash, int efs_pos, int log_pos);
char *createOutOfBandHash(	unsigned char *hash, int *errMsg );


//------------------------------------------------------
//
//	MIME Content :
//		application/vnd.wap.signed-certificate
//
//	- create signed certificate
//	- parse signeded certificate
//	- verify signeded certificate
//
//	2001.6. by Ji-Hyun Sohn
//
//------------------------------------------------------

/* enum { anonymous(0), ecdsa_sha(1), rsa_sha(2), (255) } 
   SignatureAlgorithm; */
#ifndef __WTLS__SIGNATURE_ALGORITHM__
#define __WTLS__SIGNATURE_ALGORITHM__
#define V_WTLS__ANONYMOUS					0
#define V_WTLS__ECDSA_SHA					1
#define V_WTLS__RSA_SHA						2
#endif /* __WTLS__SIGNATURE_ALGORITM__ */

typedef struct _SignedTrustedCAInfo {
    int version;
    unsigned char charSet[2];
    char *displayName;
    int trustedCACertFormat;
	int trustedCACertLen;
    unsigned char *trustedCACert;
    char *cainfo_url;
    int signerCertFormat;
	int signerCertLen;
    unsigned char *signerCert;
    int sig_alg;
    unsigned char signature[20];
} SignedTrustedCAInfo;


unsigned char *createSignedCertificate(	int version, int charSet,
										char * displayName,
										int trustedCACertFormat,
										unsigned char *trustedCACert,
										int trustedCACertLen,
										char *cainfo_url,
										int signerCertFormat,
										unsigned char *signerCert,
										int signerCertLen,
										int sig_alg, int *length,
										int *errMsg );

unsigned char *createTBSTrustedCAInfo(	int version, int charSet,
										char *displayName,
										int trustedCACertFormat,
										unsigned char *trustedCACert,
										int trustedCACertLen,
										char *cainfo_url,
										int signerCertFormat,
										unsigned char *signerCert,
										int signerCertLen,
										int sig_alg, int *length,
										int *errMsg );

unsigned char *createSignature(	unsigned char *signedCert,
								int signedCertLen, int length,
								int *errMsg );

SignedTrustedCAInfo *parseSignedCertificate(	unsigned char *signedCert,
												int signedCertLen,
												int *errMsg );

////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
//Add argments for passing EFS saving information ...swjung
int verifySignedCertificate(	unsigned char *signedCert, int signedCertLen, int efs_pos, int log_pos);

//Redefined by swjung for better fitting to SKY mobile station. Really? ^^;
int searchCACert(int keyIDType, unsigned char *keyID);

//Inserted by swjung
//Search and Return TCA, if cert is matched
unsigned char *getCACert(int keyIDType, unsigned char *keyID, int *certLen);

#ifdef BLOCK
	unsigned char *searchCACert(	int keyIDType, unsigned char *keyID,
									int keyIDLen, int *certLen,
									int *errMsg );
#endif //#ifdef BLOCK
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////

int storeNewCAInfo(	unsigned char *skid, int skidLen, char *displayName,
					unsigned char *cert, int certLen, unsigned char *akid,
					int akidLen, int cert_type, unsigned long not_after );

SignedTrustedCAInfo *new_signed_cert(void);

void free_signed_cert( SignedTrustedCAInfo *res );

#endif // WAP_MIME__H
