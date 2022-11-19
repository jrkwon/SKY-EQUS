#ifndef KEY_HASH__H
#define KEY_HASH__H


//------------------------------
//
//	key_hash.h
//
//	Calculate Key Hash
//
//	2001. 1. by Byung-Jun Yoon
//
//------------------------------


#include "Ecc.h"
#include "rsa.h"


int RSA_KeyHash(unsigned char * keyHash, RSA * rsa);


int ECC_KeyHash(unsigned char * keyHash, ECC * ecc);



//
//	End of key_hash.h
//
//------------------------

#endif	// KEY_HASH__H
