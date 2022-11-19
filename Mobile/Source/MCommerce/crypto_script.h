#ifndef CRYPTO_SCRIPT__H
#define CRYPTO_SCRIPT__H

//---------------------------------
//
//	crypto_script.h
//
//	WMLScript Crypto API
//
//	2000. 12. by Byung-Jun Yoon
//
//---------------------------------


char * script_signText( const char * strToSign, int nOption, int nKeyIdType,
						const unsigned char * keyId, int errorMMI);
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
char * script_encryptText( const char *userPrompt, const char *serverNonce,
                const char * strRecipientCert, int nKeyAgreementAlgo,
				const char * strEncAlgo, int symmetricKeyId, int errorMMI);
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

char * script_certRequest( int nCommand, const char * strSubject,
					const char * strPolicy, int errorMMI);


#endif	// CRYPTO_SCRIPT__H
