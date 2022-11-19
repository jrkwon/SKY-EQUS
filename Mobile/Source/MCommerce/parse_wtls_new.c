//------------------------------------
//
//	parse_wtls.c
//
//	 - Parse WTLS Certificate
//
//	2000. 9. by Byung-Jun Yoon
//
//------------------------------------

#include <string.h>
#include "sha.h"
#include "wcert.h"


//-------------------------
//
//	parse WTLS Certificate
//
//-------------------------

int parseWTLSCert( unsigned char * cert, long length, WTLSCert * wCert)
{
	long lOffset = 0, lLen;
	unsigned long lTime;
	int nType;
//	unsigned char md[SHA_DIGEST_LENGTH];

	memset(wCert, 0, sizeof(WTLSCert));
	
	//----------------------------
	//	1. get version
	//----------------------------
	
	wCert->version = (int) cert[lOffset++];

	//----------------------------
	//	2. get signature algorithm
	//----------------------------
	
	wCert->sigAlgo = (int) cert[lOffset++];
	
	//----------------------------
	//	3. get Issuer ID
	//----------------------------
	
	nType = (int) cert[lOffset++];

	switch( nType ){
		case ID_TYPE_TEXT:	// Identifier type is "TEXT"
			break;

		default:	// unsupported type
			return 1;
	}

	wCert->issuerCharSet[0] = cert[lOffset++];
	wCert->issuerCharSet[1] = cert[lOffset++];

	lLen = (long) cert[lOffset++];
	memcpy(wCert->issuer, &cert[lOffset], lLen);
	lOffset += lLen;
	wCert->issuer[lLen] = '\0';


	//----------------------------
	//	4. get validity period
	//----------------------------
	
	lTime =	(cert[lOffset]		<< 8*3) |
			(cert[lOffset+1]	<< 8*2) |
			(cert[lOffset+2]	<< 8*1) |
			cert[lOffset+3];

	wCert->notBefore = lTime;
	lOffset += 4;

	lTime =	(cert[lOffset]		<< 8*3) |
			(cert[lOffset+1]	<< 8*2) |
			(cert[lOffset+2]	<< 8*1) |
			cert[lOffset+3];

	wCert->notAfter = lTime;
	lOffset += 4;

	
	//----------------------------
	//	5. get Subject ID
	//----------------------------
	
	nType = (int) cert[lOffset++];

	switch( nType ){
		case ID_TYPE_TEXT:	// Identifier type is "TEXT"
			break;

		default:	// unsupported type
			return 1;
	}

	wCert->subjectCharSet[0] = cert[lOffset++];
	wCert->subjectCharSet[1] = cert[lOffset++];

	lLen = (long) cert[lOffset++];
	memcpy(wCert->subject, &cert[lOffset], lLen);
	lOffset += lLen;
	wCert->subject[lLen] = '\0';


	//----------------------------
	//	6. get Public Key
	//----------------------------
	
	wCert->pubKeyAlgo = (int) cert[lOffset++];

	switch( wCert->pubKeyAlgo ){
		case ID_PUBKEY_TYPE_RSA:	// RSA Algorithm
			break;
			
		default:	// unsupported Public Key Algorithm
			return 1;
	}

	// get parameter specifier

	nType = (int) cert[lOffset++];

	if(nType==255){ // have retrieve the parameter set
		return 1;
	}

	// get RSA exponent
	
	wCert->rsaExpLen = (cert[lOffset] << 8) |
						cert[lOffset+1];
	lOffset += 2;
	memcpy(wCert->rsaExponent, &cert[lOffset], wCert->rsaExpLen);
	lOffset += wCert->rsaExpLen;

	// get RSA modulus

	wCert->rsaModLen = (cert[lOffset] << 8) |
						cert[lOffset+1];
	lOffset += 2;
	memcpy(wCert->rsaModulus, &cert[lOffset], wCert->rsaModLen);
	lOffset += wCert->rsaModLen;

	wCert->certSize = lOffset;	// certificate size
								// (without the signature)
/*
	printf("cert size:%ld\n", wCert->certSize);
	SHA1(&cert[0], wCert->certSize, md);
	Print(md, SHA_DIGEST_LENGTH);
*/	
	//----------------------------
	//	7. get Signature
	//----------------------------
	
	wCert->sigLen = (cert[lOffset] << 8) |
					cert[lOffset+1];
	lOffset += 2;
	memcpy(wCert->signature, &cert[lOffset], wCert->sigLen);
	lOffset += wCert->sigLen;

	//----------------------------
	//	parsing completed
	//----------------------------
	
	if( length <= 0 )   return lOffset;
	if(lOffset != length){
		return 1;
	}

	// Alles klar... ^_^

	return 0;
}

RSA * getRSAPubKey( WTLSCert * wCert )
{
	int i;
	RSA * pKey = NULL;
	unsigned long e_value = 0;

	if((pKey = RSA_new())==NULL){
		return NULL;
	}

	if((pKey->e = BN_new())==NULL){
		goto err;
	}
	for(i=0; i<wCert->rsaExpLen; ++i){
		e_value |= wCert->rsaExponent[i] << 8*(wCert->rsaExpLen-i-1);
	}
	for(i=0; i<sizeof(unsigned long)*8; ++i){
		if(e_value &(1<<i)){
			BN_set_bit(pKey->e,i);
		}
	}
	
	if((pKey->n = BN_bin2bn(wCert->rsaModulus, wCert->rsaModLen, pKey->n))==NULL){
		goto err;
	}


	// Alles klar!
	
	return pKey;

err:

	if(pKey){
		RSA_free(pKey);
	}

	return NULL;

}
