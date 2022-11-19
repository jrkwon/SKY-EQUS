#ifndef HEADER_ECDSA_H
#define HEADER_ECDSA_H

#include "Ecc.h"
#include "rand.h"
#include "sha.h"

/*
 * Made by Suengchur Pyun!!
 * 
 * e-mail : pyun@postech.edu
 */

#ifdef  __cplusplus
extern "C" {
#endif

typedef struct _ecdsasig
{
	BIGNUM *r;
	BIGNUM *s;
}ECDSA_SIG;

ECDSA_SIG *ECDSA_SIG_new(void);
void ECDSA_SIG_free(ECDSA_SIG *r);

ECDSA_SIG *ECDSA_do_sign(const unsigned char *M, int bytes, ECC *ecc);
int ECDSA_do_verify(const unsigned char *M, int bytes, ECDSA_SIG *signature, ECC *ecc);

int ECC_sign(const unsigned char *message, int mlen,
			 unsigned char *sig, int *siglen, ECC *ecc);
int ECC_verify(const unsigned char *message, int mlen,
			   unsigned char *sig, int siglen, ECC *ecc);

int i2d_ECDSA_SIG(ECDSA_SIG *a, unsigned char **pp);
ECDSA_SIG *d2i_ECDSA_SIG(ECDSA_SIG **v, unsigned char **pp, long length);

int ECC_size(ECC *r);

#ifdef  __cplusplus
}
#endif
#endif

