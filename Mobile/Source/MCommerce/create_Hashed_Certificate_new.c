//-------------------------------
//
//  createHashedCertificate.c
//
//  Create a Hashed-Certificate
//
//  2001. 5. created by Ji-Hyun Sohn
//  2001. 6. modified by Ji-Hyun Sohn
//
//-------------------------------

#include <string.h>
#include "crypto.h"
#include "wap_mime.h"

unsigned char *createHashedCertificate( int version, int charSet, 
		char *displayName, int trustedCACertFormat, 
		unsigned char *cert, int certLen, char *cainfo_url, int hash_alg,
		int *length, int *errMsg ) {
	unsigned char *tbhCert = NULL;
	long nOffset = 0;
	int nTemp = 0;

	//-------------------------------------
	//	Total Length of the tbhCert
	//-------------------------------------

	*length = 1 						// version
	     	+ 2 						// character set
	     	+ 1 + strlen( displayName ) // length + display name
	     	+ 1 + certLen 				// format + certificate
	     	+ 1 + strlen( cainfo_url ) 	// length + cainfo_url
	     	+ 1; 						// hash algorithm

	if( (tbhCert = (unsigned char *)Malloc(*length)) == NULL ) {
	    	goto err1;
	}

	//-------------------------------------
	//	1. set version
	//-------------------------------------
	tbhCert[nOffset++] = version;

	//-------------------------------------
	//	2. character set
	//-------------------------------------
	tbhCert[nOffset++] = charSet >> 8;
	tbhCert[nOffset++] = charSet;

	//-------------------------------------
	//	3. display name
	//-------------------------------------
	if( (nTemp = strlen( displayName )) > 255 ) {
	    	goto err2;
	}

	tbhCert[nOffset++] = nTemp;
	WRITE_TO_BUF( tbhCert, displayName, nTemp, nOffset );

	//-------------------------------------
	//	4. certificate
	//-------------------------------------
	tbhCert[nOffset++] = trustedCACertFormat;

	switch( trustedCACertFormat ) {
		case V_WTLS_WTLSCERT:
			break;
		case V_WTLS_X509CERT:
			tbhCert[nOffset++] = certLen >> 8;
			tbhCert[nOffset++] = certLen;
			*length += 2;
			break;
		case V_WTLS_X968CERT:
		case V_WTLS_CERTURL:
		default:
			goto err3;
	}

	WRITE_TO_BUF( tbhCert, cert, certLen, nOffset );

	//-------------------------------------
	//	5. cainfo url
	//-------------------------------------
	if( (nTemp = strlen( cainfo_url )) > 255 ) {
	    	goto err2;
	}
	tbhCert[nOffset++] = nTemp;
	WRITE_TO_BUF( tbhCert, cainfo_url, nTemp, nOffset );

	//-------------------------------------
	//	6. hash algorithm
	//-------------------------------------
	tbhCert[nOffset++] = hash_alg;

	//-------------------------------------
	//	result check
	//-------------------------------------
	if( nOffset != *length ) {
	    goto err4;
	}

	*errMsg = 0;
	return tbhCert;

err1:
	*errMsg = 301; // Memory Shortage
	goto err;

err2:
	*errMsg = 302; // Larger String
	goto err;

err3:
	*errMsg = 303; // Invalid Certificate Format
	goto err;

err4:
	*errMsg = 304; // Invalid MIME Data
	goto err;

err:
	if( tbhCert ) {
	    Free( tbhCert );
	}
	return NULL;	
}

