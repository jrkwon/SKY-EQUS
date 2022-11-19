#include "Ecdsa.h"

/*
 * Made by Suengchur Pyun!!
 * 
 * e-mail : pyun@postech.edu
 */

ECDSA_SIG *ECDSA_SIG_new()
{
	ECDSA_SIG *ret;
	ret = (ECDSA_SIG *)Malloc(sizeof(ECDSA_SIG));
	if(ret == NULL) return NULL;

	ret->r = NULL;
	ret->s = NULL;

	return ret;
}

void ECDSA_SIG_free(ECDSA_SIG *r)
{
	if(r->r) BN_clear_free(r->r);
	if(r->s) BN_clear_free(r->s);
	Free(r);
}

ECDSA_SIG *ECDSA_do_sign(const unsigned char *M, int bytes, ECC *ecc) 
{
	int tmp;
	BIGNUM *e, *k, *r, *kInv, *s;
	BIGNUM *temp0;
	BN_CTX *ctx=NULL, *ctx1=NULL;
	ECDSA_SIG *result=NULL;
	Point R;

	result = ECDSA_SIG_new();
	result->r = BN_new(); result->s = BN_new();

	ctx = BN_CTX_new(); ctx1 = BN_CTX_new();

	BN_CTX_start(ctx);
	e		= BN_CTX_get(ctx); k		= BN_CTX_get(ctx);
	r		= BN_CTX_get(ctx); kInv	= BN_CTX_get(ctx);
	s		= BN_CTX_get(ctx); temp0= BN_CTX_get(ctx);

	BN_bin2bn(M, bytes, e);

	tmp = BN_num_bits(ecc->order);
	while(1)
	{
		while(1)
		{
#if 1
			if(!BN_rand(k, tmp-1, 0, 0)) goto signErr;
#else 
			BN_dec2bn(&(k), "3615660868942864851152111262419337164722625440032");
#endif
			if(!BrickellMul(&R, ecc->base, k, ecc->order, ecc->curve)) goto signErr;
			if(!Field2Bn(temp0, R.x)) goto signErr;
			if(!BN_mod(r, temp0, ecc->order, ctx1))  goto signErr;
			if(!BN_is_zero(r)) break;
		}

		if(!BN_mod_inverse(kInv, k, ecc->order, ctx1)) goto signErr;
		if(!BN_mod_mul(s, ecc->priv_key, r, ecc->order, ctx1)) goto signErr;
		if(!BN_add(temp0, s, e)) goto signErr;
		if(!BN_mod_mul(s, temp0, kInv, ecc->order, ctx1)) goto signErr;
		if(!BN_is_zero(s)) break;
	}

	BN_copy(result->r, r); BN_copy(result->s, s);
	
	BN_CTX_end(ctx); BN_CTX_free(ctx);
	BN_CTX_free(ctx1);

	return result;

signErr:

	if(result) ECDSA_SIG_free(result);
	if(ctx) BN_CTX_end(ctx);
	if(ctx) BN_CTX_free(ctx);
	if(ctx1) BN_CTX_free(ctx1);

	return NULL;
}

int ECDSA_do_verify(const unsigned char *M, int bytes, ECDSA_SIG *sig, ECC *ecc)
{
	BIGNUM *e, *c, *v;
	BIGNUM *temp[2];
	Point R[3];
	BN_CTX *ctx=NULL, *ctx1=NULL;

	ctx = BN_CTX_new(); ctx1 = BN_CTX_new();

	BN_CTX_start(ctx);
	e	= BN_CTX_get(ctx);	c	= BN_CTX_get(ctx);
	v	= BN_CTX_get(ctx);	
	temp[0] = BN_CTX_get(ctx);	temp[1] = BN_CTX_get(ctx);

	if(!BN_bin2bn(M, bytes, e)) goto verifyErr;

	if(BN_cmp(sig->r, ecc->order)>=0 || BN_is_zero(sig->r)) goto verifyErr;
	if(BN_cmp(sig->s, ecc->order)>=0 || BN_is_zero(sig->s)) goto verifyErr;

	if(!BN_mod_inverse(c, sig->s, ecc->order, ctx1)) goto verifyErr;
	if(!BN_mod_mul(temp[0], e, c, ecc->order, ctx1)) goto verifyErr;
	if(!BN_mod_mul(temp[1], sig->r, c, ecc->order, ctx1)) goto verifyErr;

	if(!BrickellMul(&R[0], ecc->base, temp[0], ecc->order, ecc->curve)) goto verifyErr;
	if(!PMul(&R[1], ecc->pub_key, temp[1], ecc->order, ecc->curve)) goto verifyErr;
	if(!Eadd(&R[2], &R[1], &R[0], ecc->curve)) goto verifyErr;

	if(!Field2Bn(temp[0], R[2].x)) goto verifyErr;
	if(!BN_mod(v, temp[0], ecc->order, ctx1)) goto verifyErr;

	if(BN_cmp(sig->r, v)!=0) goto verifyErr;

	BN_CTX_end(ctx);	BN_CTX_free(ctx);
	BN_CTX_free(ctx1);
	return 1;

verifyErr:

	if(ctx) BN_CTX_end(ctx);	
	if(ctx) BN_CTX_free(ctx);
	if(ctx1) BN_CTX_free(ctx1);
	return 0;
}

int ECC_sign(const unsigned char *message, int mlen,
			 unsigned char *sig, int *siglen, ECC *ecc)
{
	ECDSA_SIG *s=NULL;

	s = ECDSA_do_sign(message, mlen, ecc);
	if(s == NULL)
	{
		*siglen = 0;
		return 0;
	}
	*siglen = i2d_ECDSA_SIG(s, &sig);
	if(s) ECDSA_SIG_free(s);
	return 1;
}

int ECC_verify(const unsigned char *message, int mlen,
			   unsigned char *sig, int siglen, ECC *ecc)
{
	ECDSA_SIG *s=NULL;
	int ret = 0;

	s = ECDSA_SIG_new();
	if(s == NULL)
	{
		return ret;
	}
	if(d2i_ECDSA_SIG(&s, &sig, siglen) == NULL)
	{
		goto err;
	}
	ret = ECDSA_do_verify(message, mlen, s, ecc);
err:
	if(s) ECDSA_SIG_free(s);
	return ret;
}

int ECC_size(ECC *r)
{
	int ret, i;
	ASN1_INTEGER bs;
	unsigned char buf[4];

	i = 163;
	bs.length = (i+7)/8;
	bs.data=buf;
	bs.type=V_ASN1_INTEGER;
	buf[0]=0xff;

	i=i2d_ASN1_INTEGER(&bs, NULL);
	i+=i;
	ret=ASN1_object_size(1,i,V_ASN1_SEQUENCE);
	return ret;
}
