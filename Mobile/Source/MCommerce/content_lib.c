
#include <string.h>
#include <crypto.h>
#include "encrypttext.h"

PlainContent *PlainContent_new( PlainContent **a ) {
	if( (*a = (PlainContent *)Malloc(sizeof(PlainContent))) == NULL ) {
		return NULL;
	}

	(*a)->version = 1;
    (*a)->seq_num = 0;
	(*a)->content_type = V_WTLS__CT_TEXT;
	(*a)->content_encoding[0] = 0x00;
	(*a)->content_encoding[1] = 0x04;
	(*a)->subcontentList = NULL;

    return *a;
}

void PlainContent_free( PlainContent *a ) {
    if( a == NULL )
        return;
    if( a->subcontentList )
        Free( a->subcontentList );
    if( a )
        Free( a );
}

int i2d_PlainContent( PlainContent *a, unsigned char **pp ) {
	int length, nOffset = 0;

	// total length
	length 	= 1							// version
			+ 1							// seq_num
			+ 20						// server nonce
			+ 1							// content type
			+ 2							// content encoding
			+ 2 + a->subcontentList_len; 	// subContentList

	if( (*pp = (unsigned char *)Malloc(length)) == NULL ) {
		return 0;
    }

	// ----------------------------------------------------------
	// 1. Version
	// ----------------------------------------------------------
	(*pp)[nOffset++] = a->version;

	// ----------------------------------------------------------
	// 2. Sequence Number
	// ----------------------------------------------------------
	(*pp)[nOffset++] = a->seq_num;

	// ----------------------------------------------------------
	// 3. Server Nonce
	// ----------------------------------------------------------
	memcpy(*pp + nOffset, a->serverNonce, 20);
	nOffset = nOffset + 20;

	// ----------------------------------------------------------
	// 4. Content Type
	// ----------------------------------------------------------
	(*pp)[nOffset++] = a->content_type;

	// ----------------------------------------------------------
	// 5. Content Encoding
	// ----------------------------------------------------------
	memcpy(*pp + nOffset, a->content_encoding, 2);
	nOffset = nOffset + 2;

	// ----------------------------------------------------------
	// 6. SubContentList
	// ----------------------------------------------------------
	(*pp)[nOffset++] = a->subcontentList_len >> 8;
	(*pp)[nOffset++] = a->subcontentList_len;

	memcpy(*pp + nOffset, a->subcontentList, a->subcontentList_len);
	nOffset = nOffset + a->subcontentList_len;

	if( nOffset != length ) {
    	goto err;
    }

    return length;

err:
    if( *pp ) Free( *pp );
    return 0;
}

PlainContent *PlainContent_set( PlainContent **a, int seq,
		unsigned char *nonce, int type, 
		unsigned char *contentList, int contentList_len ) {

	if( PlainContent_new( a ) == NULL ) {
		return NULL;
	}
	
	(*a)->seq_num = seq;
	memcpy((*a)->serverNonce, nonce, 20);

	switch( type ) {
		case V_WTLS__CT_TEXT :
			(*a)->content_type = type;
			break;
		case V_WTLS__CT_DATA :
		default :
			goto err;
	}

	(*a)->subcontentList_len = contentList_len;

	if( ((*a)->subcontentList = (unsigned char *)Malloc(contentList_len)) == NULL ) {
		goto err;
	}

	memcpy((*a)->subcontentList, contentList, contentList_len);

	return *a;

err:
	if( *a )
		PlainContent_free( *a );
	return NULL;
}

