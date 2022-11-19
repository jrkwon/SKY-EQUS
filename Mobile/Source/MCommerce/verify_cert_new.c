//---------------------------------------------------------------------------
//
//	verify_cert.c
//
//	2001.06.20
//
//	by Kim Jae Hyun
//
//---------------------------------------------------------------------------

#include	"sha.h"
#include	"rsa.h"
#include	"crypto.h"
#include	"wcert.h"
#include	"util.h"


//swjung
//Add local SKY timer operation
#include "sky.h"

#define		FIND_DEPTH	1


int verifyWTLSCert( unsigned char * cert, long certLen, RSA * caPubKey ){
	WTLSCert wcert;
	unsigned char	dec_sig[SHA_DIGEST_LENGTH], md[SHA_DIGEST_LENGTH];
	int		ret;

	//swjung exclude time_t
	//time_t	curTime;
	unsigned long curTime;

	// nx: temparary
	char	strTime[128];

	//-----------------------------------------------
	// parse user_cert and check validity
	//-----------------------------------------------

	if( (ret=parseWTLSCert(cert, certLen, &wcert)) ){
		return 1081;
	}

	//curTime = SKY_GetLocalTime();
	curTime = CLNTa_currentTime();
	
	if( !(wcert.notBefore < curTime || curTime < wcert.notAfter) ){
		return 1082;
	}

	switch( wcert.sigAlgo ){
		case ID_SIGALGO_RSA_SHA:	break;
		default:			// unsupported algorithm
			return 1083;
	}


	//-----------------------------------------------
	// decrypt the signature with the CA Public Key
	//-----------------------------------------------

	//if( (ret = RSA_public_decrypt(wcert.sigLen, wcert.signature,
	if( (ret = rsaPublicDecrypt(wcert.sigLen, wcert.signature,
				   	dec_sig, caPubKey, RSA_PKCS1_PADDING)) == -1 ){
		return 1084;
	}
	if( ret != SHA_DIGEST_LENGTH ){
		return 1085;
	}


	//-----------------------------------------------
	// get the finger-print of the user certificate
	//-----------------------------------------------

	SHA1( cert, wcert.certSize, md );


#ifdef BLOCK
	if( memcmp(md, dec_sig, SHA_DIGEST_LENGTH) ){
		return 1086;
	}
#endif //#ifdef BLOCK

	return 0;
}


int verifyCert( int certFmt, unsigned char * userCert, long userCertLen ){
	WTLSCert	* caCert =NULL;
	RSA	* caPubKey = NULL;
	unsigned char	keyid[512];

	int		buflen, buf2len;
	unsigned char	* buf, * buf2;

	//unsigned char	* skid, *akid;

	int		i, ret;


	// keyID length is needed

	//if( keyID != NULL )	memcpy( keyid, keyID, 20 );

	//------------------------------------------
	// read CA Certificate by 'keyid'
	//------------------------------------------
	//	i:	keyid
	//	o:	ca_cert(buf2, buf2len)
	//------------------------------------------

	if( (caCert = (WTLSCert *)Malloc(sizeof(WTLSCert))) == NULL ){
		//printf( "cannot mem alloc\n" );
		return 1002;
	}


	if( parseWTLSCert(userCert, userCertLen, caCert) ){
		return 1003;
	}


	// get akid

	memcpy( keyid, caCert->issuer, sizeof(caCert->issuer) );


	if( (buf2 = getCACertificate(certFmt, (const unsigned char *)keyid, &buf2len)) == (unsigned char *)NULL ){
		return 1001;
	}


	buf = userCert;
	buflen = userCertLen;

	//==========================================
	// Chain Verify
	//	buf: user
	//	buf2: ca
	//==========================================

	for( i=0 ; i<FIND_DEPTH ; i++ ){

		//------------------------------------------
		// parse Certificates, get Ca Public Key
		// and verify user_cert
		//------------------------------------------
		//	i:	buf2
		//	o:	caCert, caPubKey
		//------------------------------------------

		//PrintData(buf2, buf2len);

		/////////////////////////////////////////////////////////////////
		//Inserted to get cert length by swjung 2001.10.15
		/////////////////////////////////////////////////////////////////
		buf2len = parseWTLSCert(buf2, 0, caCert); 

		if( parseWTLSCert(buf2, buf2len, caCert) ){
			return 1003;
		}

		//PrintWTLSCert( caCert );

		if( (caPubKey = getRSAPubKey(caCert)) == NULL ){
			return 1004;
		}

		if( (ret = verifyWTLSCert(buf, buflen, caPubKey)) ){
			return ret;
		}


		//------------------------------------------
		// check ca_cert is root_certificate
		//------------------------------------------

		//printf( ".[32m%s\n.[36m%s.[37m\n\n", caCert->subject, caCert->issuer );
		if( !memcmp(caCert->subject, caCert->issuer, 20) || caCert->issuer == NULL ){
			return 0;		// rootCA, SUCCESS
		}

		//------------------------------------------
		// read CA Certificate by 'keyid'
		//------------------------------------------

		buf = buf2;
		buflen = buf2len;
		buf2 = NULL;		// nxq
		buf2len = 0;

		// nx
		// issuer의 length로 바꿔야한다.
#if 0
		if( keyid == NULL )	Free( keyid );
		if( (keyid = (unsigned char *)Malloc(sizeof(caCert->issuer))) ){
			return 6;
		}
#endif
		memcpy( keyid, caCert->issuer, sizeof(caCert->issuer) );

		if( (buf2 = getCACertificate(certFmt, keyid, &buf2len)) == NULL ){
			return 1007;
		}
	}

	if ( caPubKey )	RSA_free( caPubKey );
	if ( caCert )	Free( caCert );
	if ( buf )		Free( buf );
	if ( buf2 )		Free( buf2 );
	return 0;
}


//
//	End of verify_cert.c
//
//---------------------------------------------------------------------------
