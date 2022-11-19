//-------------------------------
//
//  parseHashedCertificate.c
//
//  - parse the Hashed-Certificate
//
//  2001. 5. created by Ji-Hyun Sohn
//  2001. 6. modified by Ji-Hyun Sohn
//
//-------------------------------

#include <string.h>
#include "crypto.h"
#include "wap_mime.h"
#include "wcert.h"

TBHTrustedCAInfo *new_hashed_cert(void);
void free_hashed_cert( TBHTrustedCAInfo *res );

TBHTrustedCAInfo *parseHashedCertificate( unsigned char *hashedCert,
		int hashedCertLen, int *errMsg ) {
	TBHTrustedCAInfo *res = NULL;
	int nOffset = 0, nTemp;
	WTLSCert wcert;

	if( (res = new_hashed_cert()) == NULL ) {
	    goto err1;
    }

    //-------------------------------------
    //	1. get version
    //-------------------------------------
    res->version = hashedCert[nOffset++];

    //-------------------------------------
    //	2. get display name
    //-------------------------------------
	READ_FROM_BUF( hashedCert, res->charSet, 2, nOffset ); // character set

	nTemp = hashedCert[nOffset++];

	if( (res->displayName = (char *)Malloc( nTemp+1 )) == NULL ) {
	    goto err1;
	}

    READ_FROM_BUF( hashedCert, res->displayName, nTemp, nOffset ); // display name
    res->displayName[nTemp] = '\0';

    //-------------------------------------
    //	3. get trustedCACert
    //-------------------------------------
	res->trustedCACertFormat = hashedCert[nOffset++]; // certificate format

	switch( res->trustedCACertFormat ) {
	    case V_WTLS__WTLS_CERT :
	        nTemp = parseWTLSCert( hashedCert +nOffset, 0, &wcert );
	        break;
	    case V_WTLS__X509_CERT :
	        nTemp = hashedCert[nOffset++] << 8;
	        nTemp |= hashedCert[nOffset++];
	        break;
	    case V_WTLS__X968_CERT :
	    case V_WTLS__CERT_URL :
	    default :
	        goto err2;
	}

	// added in order to get the length of the trustedCACert
	res->trustedCACert_len = nTemp;
    if( (res->trustedCACert = (unsigned char *)Malloc( nTemp )) == NULL ) {
        goto err1;
    }

    READ_FROM_BUF( hashedCert, res->trustedCACert, nTemp, nOffset ); // certificate
//    res->trustedCACert[nTemp] = '\0';

    //-------------------------------------
    //	4. get cainfo url
    //-------------------------------------
	nTemp = hashedCert[nOffset++];

	if( (res->cainfo_url = (char *)Malloc( nTemp+1 )) == NULL ) {
	    goto err1;
	}

	READ_FROM_BUF( hashedCert, res->cainfo_url, nTemp, nOffset );
	res->cainfo_url[nTemp] = '\0';

    //-------------------------------------
    //	5. get hash algorithm
    //-------------------------------------
    res->hash_alg = hashedCert[nOffset++];

    //-------------------------------------
    //	result check
    //-------------------------------------
    if( nOffset != hashedCertLen ) {
        goto err3;
    }

	*errMsg = 0; // Success

    return res;

err1:
	*errMsg = 10020001; // Memory Allocation Error
	goto err;

err2:
	*errMsg = 10020003; // Unsupported Certificate Format
	goto err;

err3:
	*errMsg = 10020003; // Invalid MIME Data
	goto err;

err:
	if( res ) {
	    free_hashed_cert( res );
	}
	return NULL;
}

TBHTrustedCAInfo *new_hashed_cert() {
	TBHTrustedCAInfo *res;

	if( (res = (TBHTrustedCAInfo *)Malloc( sizeof( TBHTrustedCAInfo ) )) == NULL ) {
        return NULL;
    }

    res->displayName = NULL;
    res->trustedCACert = NULL;
    res->cainfo_url = NULL;

    memset( res->charSet, 0, 2 );

    res->version = 0;
    res->trustedCACertFormat = 0;
    res->hash_alg = -1;

    return res;
}

void free_hashed_cert( TBHTrustedCAInfo *res ) {
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
	    Free( res );
    }
}

