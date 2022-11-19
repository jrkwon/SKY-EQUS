#ifndef ENCRYPTTEXT__H
#define ENCRYPTTEXT__H

//---------------------------
//
//	encrypttext.h
//
//	2000. 12
//
//	by Byung-Jun Yoon
//
//----------------------------

#include "script_enum.h"

/* 2001.09.27
 *	- encryptTextEx 9/27 Spec 적용
 * 	- prototype 변경
 */

// Content Type
enum {
	V_WTLS__CT_TEXT = 1,
	V_WTLS__CT_DATA = 2
};

typedef struct _palin_content {
	int version;
	int seq_num;
	unsigned char serverNonce[20];
	int content_type;
	unsigned char content_encoding[2];
	int subcontentList_len;
	unsigned char *subcontentList;
} PlainContent;


unsigned char * encryptText( const unsigned char * strToEnc, int nStrLen,
		const unsigned char * cert, int nCertLen, int nPubKeyAlgo,
	    const char * strEncAlgo, int * length);

PlainContent *PlainContent_new( PlainContent **a );
void PlainContent_free( PlainContent *a );
int i2d_PlainContent( PlainContent *a, unsigned char **pp );
PlainContent *PlainContent_set( PlainContent **a, int seq,
		unsigned char *nonce, int type,
		unsigned char *contentList, int contentList_len );

// <-- 2001.09.28


unsigned char * decryptText( const unsigned char * envelope, int nEnvLen, int * length);

PlainContent *d2i_PlainContent( PlainContent **a, unsigned char *pp, int *length );
void PlainContent_print( unsigned char *pp );

//
//	End of encrypttext.h
//
//-------------------------

#endif // ENCRYPTTEXT__H
