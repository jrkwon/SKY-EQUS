#ifndef RSA_H
#define RSA_H
#include "bn.h"

typedef struct rsa_st {
	BIGNUM *n;
	BIGNUM *e;
	BIGNUM *d;
	BIGNUM *p;
	BIGNUM *q;
	BIGNUM *dmp1;
	BIGNUM *dmq1;
	BIGNUM *iqmp;
} RSA;

RSA *	IA_RSA_New(void);
void	IA_RSA_Free (RSA *r);
int	IA_RSA_PublicEncrypt(int flen, unsigned char *from,
		unsigned char *to, RSA *rsa);

/* Random Number Functions */

#endif

