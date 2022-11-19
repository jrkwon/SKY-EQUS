//------------------------------
//
//	key_hash.c
//
//	Calculate Key Hash
//
//	2001. 1. by Byung-Jun Yoon
//
//------------------------------

#include "Ecc.h"
#include "rsa_new.h"
#include "bn_new.h"
#include "sha.h"



// For RSA, the SHA-1 hash is to be done on the byte
// string representation of the public moulus [PKCS1]


void RSA_KeyHash(unsigned char * keyHash, RSA * rsa)
{
	unsigned char key[256];
	int len;

	len = BN_bn2bin(rsa->n, key);

//	printf("RSA mod:%d\n", len);
//	PrintData(key, len);

	SHA1(key, len, keyHash);

}



// For ECC, the SHA-1 hash is to be done on the byte
// string representation of the x-coordinate of the
// elliptic curve point [X9.62]


void ECC_KeyHash(unsigned char * keyHash, ECC * ecc)
{

	unsigned char key[48];
	int fsize, ol;

	fsize = 163;
	ol = (fsize+7)/8;

	Field2Octet(key, ecc->pub_key->x, fsize);

//	printf("ECC x:%d\n", ol);
//	PrintData(key, ol);

	SHA1(key, ol, keyHash);

}


//
//	End of key_hash.c
//
//------------------------
