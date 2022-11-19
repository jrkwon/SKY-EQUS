#ifndef KEY_OP__H
#define KEY_OP__H

#include <seed.h>
#include <sha.h>
#include <rsa.h>

#ifndef SEED_BLOCK_SIZE
#define SEED_BLOCK_SIZE 16
#endif

#ifndef SHA_DIGEST_LENGTH
#define SHA_DIGEST_LENGTH 20
#endif

#ifndef DEBUG
#define DEBUG 0
#endif

unsigned char *key_encrypt( unsigned char * plainStr, int nPlainStr,
        unsigned char *pin, int *nCipherLen );
unsigned char *key_decrypt( unsigned char * cipherStr, int nCipherStr,
        unsigned char *pin, int *nPlainStr );

#ifdef SCRIPT_ECC
ECC * getPrivateKeyFromDN(const char *dn);
#endif

//void print_result( unsigned char *data, int nLen );

#endif // KEY_OP__H
