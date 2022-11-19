//------------------------------------------------------
//
//      seed_encrypt.c
//
//      - encrypt using SEED
//
//      2001.6. by Ji-Hyun Sohn
//
//-----------------------------------------------------

#include "Ecc.h"
#include "key_op.h"
#include "crypto_size.h"

#include "clntwallet.h"

unsigned char *key_encrypt( unsigned char *plainStr, int nPlainStr,
        unsigned char *pin, int *nCipherStr ) {
    SEED_KEY_SCHEDULE ks;
    SEED_CBLOCK key, iv;
    unsigned char *cipherStr = NULL;

    *nCipherStr = seed_enc_block_size_with_padding( nPlainStr );

    if( (cipherStr = (unsigned char *)Malloc(*nCipherStr)) == NULL ) {
        goto err;
    }

    memcpy( key, pin, SEED_BLOCK_SIZE );
    seedSetKey( &key, ks );

    memcpy( iv, pin, SEED_BLOCK_SIZE );

    if( seedEncryptCBC( plainStr, cipherStr, nPlainStr, ks, iv ) != *nCipherStr ) {
        goto err;
    }

    return cipherStr;

err:
    if( cipherStr ) {
        Free( cipherStr );
    }
    return NULL;
}


unsigned char *key_decrypt( unsigned char *cipherStr, int nCipherStr,
        unsigned char *pin, int *nPlainStr ) {
    SEED_KEY_SCHEDULE ks;
    SEED_CBLOCK key, iv;
    unsigned char *plainStr = NULL;

    memcpy( key, pin, SEED_BLOCK_SIZE );
    seedSetKey( &key, ks );
    memcpy( iv, pin, SEED_BLOCK_SIZE );

    if( (plainStr = (unsigned char *)Malloc(nCipherStr)) == NULL ) {
        goto err;
    }

    if( (*nPlainStr = seedDecryptCBC(cipherStr, plainStr, nCipherStr, ks, iv)) == -1 ) {
        goto err;
    }

    return plainStr;

err:
    if( plainStr ) {
        Free( plainStr );
    }
    return NULL;
}

#ifdef SCRIPT_ECC
#ifdef BLOCK
ECC * getPrivateKeyFromDN(const char *dn)
{
	ECC * key = NULL;
	unsigned char * buf = NULL;
	int length = 96;
	int nDecPrvKey;
	char *pin = NULL;
	unsigned char pin_hash[20];
	unsigned char *decPrvKey = NULL;


	buf = get_encPrvKeyFromDN(dn); // Storage Function
	pin = getPin_ofPrivateKey();
	SHA1(pin, 8, pin_hash);
	decPrvKey = key_decrypt(buf, length, pin_hash, &nDecPrvKey);

	if(!d2i_ECCPrivateKey(&key, &decPrvKey, nDecPrvKey)){
	    return NULL;
	}

	return key;
}
#endif //#ifdef BLOCK
#endif  // SCRIPT_ECC

