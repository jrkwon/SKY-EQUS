//-------------------------------
//
//  createSignedCertificate.c
//
//  Create To Be Signed Certificate
//
//  2001. 6. created by Jae-Hyun Kim
//  2001. 6. modified by Ji-Hyun Sohn
//
//-------------------------------

#include "string.h"
#include "crypto.h"
#include "sha.h"
#include "wap_mime.h"
#include "sky.h"

/*
	WARN:
		createHashedCertificate와 달리 createSignedCertificate는
		SignedTrustedCAInfo structure를 return한다.
		signature는 TBSTrustedCAInfo의 WTLS encoding된 값에 대한 sign이다.
		
struct {
	uint8			version; 
	CertDisplayName		displayName;
	Certificate		trustedCACert;
	opaque			cainfo_url <0..2^8-1>;
	Certificate		signerCert;
	SignatureAlgorithm	sig_alg;
} TBSTrustedCAInfo;

struct {
	TBSTrustedCAInfo	tc_info;
	Signature		signature; 
} SignedTrustedCAInfo
*/

unsigned char *createSignedCertificate( int version, int charSet,
		char * displayName, int trustedCACertFormat,
		unsigned char *trustedCACert, int trustedCACertLen,
		char *cainfo_url, int signerCertFormat,
		unsigned char *signerCert, int signerCertLen,
		int sig_alg, int *length, int *errMsg ) {

	unsigned char * buf, * ret;
	unsigned char *signature = NULL;
	int		i;

	buf = createTBSTrustedCAInfo( version, charSet, displayName,
			trustedCACertFormat, trustedCACert, trustedCACertLen,
		    cainfo_url, signerCertFormat, signerCert, signerCertLen,
			sig_alg, length, errMsg );

	if( (ret = (unsigned char *)Malloc(*length +20)) == NULL ) {
		*errMsg = 411; // Memory
		goto	err;
	}

	memcpy( ret, buf, *length );

	switch( sig_alg ) {
		case V_WTLS__ANONYMOUS :
			break;
		case V_WTLS__ECDSA_SHA :
		case V_WTLS__RSA_SHA :
			if( (signature = (unsigned char *)Malloc(SHA_DIGEST_LENGTH)) == NULL ) {
				*errMsg = 411; // Memory Shortage
				return NULL;
    		}

			SHA1( buf, *length, signature );

			for( i = 0; i < SHA_DIGEST_LENGTH; i++ )
				ret[*length + i] = signature[i];

			*length += 20;
			break;
		default :
			*errMsg = 412;
			return NULL;
	}

	*errMsg = 0; // Success

err:
	return ret;
}

unsigned char *createTBSTrustedCAInfo( int version, int charSet, 
		char *displayName, int trustedCACertFormat, unsigned char *trustedCACert,
		int trustedCACertLen, char *cainfo_url, int signerCertFormat,
	    unsigned char *signerCert, int signerCertLen, int sig_alg,
		int *length, int *errMsg ) {

	unsigned char *tbsCert = NULL;
	long lOffset = 0, lLen;
	int nTemp = 0;

	//-------------------------------------
	//	Total Length of the tbsCert
	//-------------------------------------

	lLen = 1 			// version
	     + 2 			// character set
	     + 1 + strlen( displayName ) 	// character set + display name
	     + 1 + trustedCACertLen 		// format + WTLSCertificate
	     + 1 + strlen( cainfo_url ) 	// length + cainfo_url
	     + 1 + signerCertLen 		// format + WTLSCertificate (signer)
	     + 1; 			// hash algorithm

	if( (tbsCert = (unsigned char *)Malloc(lLen)) == NULL ) {
	    	goto err1;
	}

	//-------------------------------------
	//	1. set version
	//-------------------------------------
	tbsCert[lOffset++] = version;

	//-------------------------------------
	//	2. character set
	//-------------------------------------
	tbsCert[lOffset++] = charSet >> 8;
	tbsCert[lOffset++] = charSet;

	//-------------------------------------
	//	3. display name
	//-------------------------------------
	if( (nTemp = strlen( displayName )) > 255 ) {
	    	goto err2;
	}

	tbsCert[lOffset++] = nTemp;
	WRITE_TO_BUF( tbsCert, displayName, nTemp, lOffset );

	//-------------------------------------
	//	4. certificate
	//-------------------------------------
	tbsCert[lOffset++] = trustedCACertFormat;

	switch( trustedCACertFormat ) {
		case V_WTLS__WTLS_CERT:
			break;
		case V_WTLS__X509_CERT:
		case V_WTLS__X968_CERT:
	        tbsCert[lOffset++] = trustedCACertLen >> 8;
	        tbsCert[lOffset++] = trustedCACertLen;
			lLen += 2; // cert length
			break;
		case V_WTLS__CERT_URL:
			goto err3;
	}

	WRITE_TO_BUF( tbsCert, trustedCACert, trustedCACertLen, lOffset );

	//-------------------------------------
	//	5. cainfo url
	//-------------------------------------
	if( (nTemp = strlen( cainfo_url )) > 255 ) {
	    	goto err2;
	}
	tbsCert[lOffset++] = nTemp;
	WRITE_TO_BUF( tbsCert, cainfo_url, nTemp, lOffset );

	//-------------------------------------
	//	6. certificate (signer)
	//-------------------------------------
	tbsCert[lOffset++] = signerCertFormat;

	switch( signerCertFormat ) {
		case V_WTLS__WTLS_CERT:
			break;
		case V_WTLS__X509_CERT:
		case V_WTLS__X968_CERT:
	        tbsCert[lOffset++] = signerCertLen >> 8;
	        tbsCert[lOffset++] = signerCertLen;
			lLen += 2; // cert length
			break;
		case V_WTLS__CERT_URL:
			goto err3;
	}

	WRITE_TO_BUF( tbsCert, signerCert, signerCertLen, lOffset );

	//-------------------------------------
	//	7. signature algorithm
	//-------------------------------------
	tbsCert[lOffset++] = sig_alg;

	//-------------------------------------
	//	result check
	//-------------------------------------
	if( lOffset != lLen ) {
	    goto err4;
	}

	*errMsg = 0;
	*length = lLen;

	return tbsCert;

err1:
	*errMsg = 411; // Memory Allocation Error
	goto err;

err2:
	*errMsg = 413; // String is lager than 255
	goto err;

err3:
	*errMsg = 414; // Certificate Format Error
	goto err;

err4:
	*errMsg = 415; // Result doesn't match
	goto err;

err:
	if( tbsCert ) {
	    Free( tbsCert );
	}
	return NULL;	
}

// signedCertLen : 전체 SignedTrustedCAInfo의 길이
// length : TBSTrustedCAInfo의 길이
// 따라서 signedCertLen - length : Signature의 길이

unsigned char *createSignature( unsigned char *signedCert, int signedCertLen,
		int length, int *errMsg ) {
	unsigned char *signature;
	int i;

	if( (signature = (unsigned char *)Malloc(signedCertLen - length)) == NULL ) {
	    goto err;
	}

	for( i = 0; i < signedCertLen - length; i++ ) {
		signature[i] = signedCert[length + i];
	}

	*errMsg = 0;
	return signature;

err:
	*errMsg = 411; // Memory
	return NULL;
}

