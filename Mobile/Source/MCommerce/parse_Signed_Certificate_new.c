//-------------------------------
//
//  parseSignedCertificate.c
//
//  - parse the Signed-Certificate mime
//
//  2001. 6. by Ji-Hyun Sohn
//
//-------------------------------

#include <string.h>
#include "crypto.h"
#include "wap_mime.h"
#include "wcert.h"

SignedTrustedCAInfo *parseSignedCertificate(	unsigned char *signedCert, 
				int signedCertLen,
				int *errMsg ) {
    SignedTrustedCAInfo *res = NULL;
    int nOffset = 0, nTemp;
    WTLSCert wcert;

    if( (res = new_signed_cert()) == NULL ) {
		goto err1;
    }

    //-------------------------------------
    //	1. get version
    //-------------------------------------
    res->version = signedCert[nOffset++];

    //-------------------------------------
    //	2. get display name
    //-------------------------------------
    READ_FROM_BUF( signedCert, res->charSet, 2, nOffset ); // character set

    nTemp = signedCert[nOffset++];

    if( (res->displayName = (char *)Malloc( nTemp+1 )) == NULL ) {
		goto err1;
    }

    READ_FROM_BUF( signedCert, res->displayName, nTemp, nOffset ); // display name
    res->displayName[nTemp] = '\0';

    //-------------------------------------
    //	3. get trustedCACert
    //-------------------------------------
    res->trustedCACertFormat = signedCert[nOffset++]; // certificate format

    switch( res->trustedCACertFormat ) {
		case V_WTLS__WTLS_CERT :
	    	nTemp = parseWTLSCert( signedCert +nOffset, 0, &wcert );
	    	break;
		case V_WTLS__X509_CERT :
            nTemp = signedCert[nOffset++] << 8;
            nTemp |= signedCert[nOffset++];
	    	break;
		case V_WTLS__X968_CERT :
		case V_WTLS__CERT_URL :
		default :
			goto err4;
    }

	if( nTemp == 1 ) goto err5;

    if( (res->trustedCACert = (unsigned char *)Malloc( nTemp )) == NULL ) {
		goto err1;
    }

    READ_FROM_BUF( signedCert, res->trustedCACert, nTemp, nOffset ); // certificate
//    res->trustedCACert[nTemp] = '\0';
	res->trustedCACertLen = nTemp;

    //-------------------------------------
    //	4. get cainfo url
    //-------------------------------------
    nTemp = signedCert[nOffset++];

    if( (res->cainfo_url = (char *)Malloc( nTemp+1 )) == NULL ) {
		goto err1;
    }

    READ_FROM_BUF( signedCert, res->cainfo_url, nTemp, nOffset );
    res->cainfo_url[nTemp] = '\0';

    //-------------------------------------
    //	5. get signerCert
    //-------------------------------------
    res->signerCertFormat = signedCert[nOffset++]; // certificate format

	switch( res->signerCertFormat ) {
		case V_WTLS__WTLS_CERT :
	    	nTemp = parseWTLSCert( signedCert +nOffset, 0, &wcert );
	    	break;
		case V_WTLS__X509_CERT :
        	nTemp = signedCert[nOffset++] << 8;
        	nTemp |= signedCert[nOffset++];
	    	break;
		case V_WTLS__X968_CERT :
		case V_WTLS__CERT_URL :
		default :
			goto err4;
    }

	if( nTemp == 1 ) goto err6;

    if( (res->signerCert = (unsigned char *)Malloc( nTemp )) == NULL ) {
		goto err1;
    }

    READ_FROM_BUF( signedCert, res->signerCert, nTemp, nOffset ); // certificate
//    res->signerCert[nTemp] = '\0';
	res->signerCertLen = nTemp;

    //-------------------------------------
    //	6. get signature algorithm
    //-------------------------------------
    res->sig_alg = signedCert[nOffset++];

    //-------------------------------------
    //	7. get signature
    //-------------------------------------
    switch( res->sig_alg ) {
		case V_WTLS__ANONYMOUS :
	    	goto err2;
		case V_WTLS__ECDSA_SHA :
		case V_WTLS__RSA_SHA :
	    	READ_FROM_BUF( signedCert, res->signature, SHA_DIGEST_LENGTH, nOffset );
	    	break;
		default : 
	    	goto err2;
    }

    //-------------------------------------
    //	result check
    //-------------------------------------
    if( nOffset != signedCertLen ) {
        goto err3;
    }

    *errMsg = 0; // Success

    return res;

err1:
	*errMsg = 10030001; // Memory Allocation Error
	goto err;

err2:
	*errMsg = 10030006; // Unsupported Signature Algorithm
	goto err;

err3:
	*errMsg = 10030005; // Invalid MIME Data
	goto err;

err4:
	*errMsg = 10030004; // Unsupported Certificate Format
	goto err;

err5:
	*errMsg = 10030007; // Invalid trustedCACert WTLS Cert
	goto err;

err6:
	*errMsg = 10030007; // Invalid signerCert WTLS Cert
	goto err;

err:
	if( res ) {
	    free_signed_cert( res );
	}
	return NULL;	
}

SignedTrustedCAInfo *new_signed_cert(void) {
    SignedTrustedCAInfo *res;

    if( (res = (SignedTrustedCAInfo *)Malloc( sizeof( SignedTrustedCAInfo ) )) == NULL ) {
		return NULL;
    }

    res->displayName = NULL;
    res->trustedCACert = NULL;
    res->cainfo_url = NULL;
    res->signerCert = NULL;

    memset( res->charSet, 0, 2 );
    memset( res->signature, 0, SHA_DIGEST_LENGTH );

    res->version = 0;
    res->trustedCACertFormat = 0;
	res->trustedCACertLen = 0;
    res->signerCertFormat = 0;
	res->signerCertLen = 0;
    res->sig_alg = -1;

    return res;
}

void free_signed_cert( SignedTrustedCAInfo *res ) {
    if( res ) {
	if( res->displayName ) {
	    Free( res->displayName );
	}
	if( res->trustedCACert ) {
	    Free( res->trustedCACert );
	}
	if( res->cainfo_url ) {
	    Free(  res->cainfo_url );
	}
	if( res->signerCert ) {
	    Free( res->signerCert );
	}
	Free( res );
    }
}

