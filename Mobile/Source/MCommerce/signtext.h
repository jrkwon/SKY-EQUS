#ifndef SIGNTEXT__H
#define SIGNTEXT__H

//---------------------------
//
//	signtext.h
//
//	2000. 10
//
//	by Byung-Jun Yoon
//
//----------------------------

#include "script_enum.h"

// signText Options

enum {  INCLUDE_CONTENT     = 0x0001,
		INCLUDE_KEY_HASH    = 0x0002,
		INCLUDE_CERTIFICATE = 0x0004 }; 

// Key ID Type

enum {  NONE                = 0x0000,
		USER_KEY_HASH       = 0x0001,
		TRUSTED_KEY_HASH    = 0x0002 };

		

void setSigTemplate( unsigned char * template, const char * strTime,
					 const unsigned char * strToSign, int nLength);
		

unsigned char * signText( const char * strToSign, int nOpt, int nKeyIdType,
							const unsigned char * keyId, int * contentLength);


int verifyText( const unsigned char * sig, int nLength, const char * content,
				const unsigned char * certificate, int nCertLength);

char * getSigningTime(void);

unsigned char * getSignerKeyHash(void);

char * getSignedContent(void);

unsigned char * getSignerCert(int * type, int * length);



//
//	End of signtext.h
//
//-------------------------

#endif // SIGNTEXT__H

