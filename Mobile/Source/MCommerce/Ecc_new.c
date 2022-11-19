#include "Ecc.h"

/*
 * Made by Suengchur Pyun!!
 * 
 * e-mail : pyun@postech.edu
 */

#ifndef NO_TIMING_ATTACK
Point Vi, Vf;
static int isInitBlinding = 0;

void blinding_init() { isInitBlinding = 0; }
/* 
 * This is a blinding method for preventing a timing attack.
 *
 * Paper : Timing Attacks on Implementations of Diffie-Hellman,
 * RSA, DSS, and Other Systems.
 * 
 * Crypto 96' : Paul C. Kocher
 *
 * r : random number 
 * Vi := r*G ( G is the base point. )
 * Vr := d*Vi ( d is private key. )
 *
 * Implemenated by Suengchur Pyun
 *
 * e-mail : pyun@postech.edu
 */

int generate_blinding_pair(ECC *ecc)
{
	BIGNUM *rand;
	int tmp;

	if(isInitBlinding)
	{
		Edbl(&Vi, &Vi, ecc->curve);		
		Edbl(&Vf, &Vf, ecc->curve);
		return 1;
	}else
	{
		if((rand = BN_new()) == NULL) return 0;
		tmp = BN_num_bits(ecc->order);
		if(!BN_rand(rand, tmp-1, 0, 0)) return 0;
		BrickellMul(&Vi, ecc->base, rand, ecc->order, ecc->curve);
		PMul(&Vf, &Vi, ecc->priv_key, ecc->order, ecc->curve);
		isInitBlinding = 1;
		return 1;
	}
}
#endif

ECC *ECC_new()
{
	ECC *ret;
	ret = (ECC *)Malloc(sizeof(ECC));
	if(ret == NULL) return NULL;

	ret->base = NULL;
	ret->curve = NULL;
	ret->order = NULL;
	ret->priv_key = NULL;
	ret->pub_key = NULL;
/*	Curve_K_163(ret); */
	Curve_5(ret);

	return ret;
}

void ECC_free(ECC *e)
{
	if(e == NULL) return;

	if(e->base) Point_free(e->base);
	if(e->curve) Curve_free(e->curve);
	if(e->order) BN_clear_free(e->order);
	if(e->priv_key) BN_clear_free(e->priv_key);
	if(e->pub_key) Point_free(e->pub_key);
}

int ECC_generate_key(ECC *A)
{
	int bits;
	BIGNUM *ret=NULL;
	Point *pret=NULL;

	if(A == NULL) return 0;

	if(A->priv_key==NULL)
	{
		ret = BN_new();
		if(ret==NULL) return 0;
		A->priv_key = ret;
	}
	
	if(A->pub_key==NULL)
	{
		if((pret = Point_new()) == NULL) goto keyErr;
		A->pub_key = pret;
	}

	bits = BN_num_bits(A->order);

	while(1)
	{
#if 1
		if(!BN_rand(A->priv_key, bits-1, 0, 0)) goto keyErr;
#else
		BN_dec2bn(&(A->priv_key), "3615660868942864851152111262419337164722625440033");
#endif
		if(BN_cmp(A->priv_key, A->order)<0 && !BN_is_zero(A->priv_key)) break;
	}
	if(!BrickellMul(A->pub_key, A->base, A->priv_key, A->order, A->curve)) goto keyErr;

#ifndef NO_TIMING_ATTACK
	isInitBlinding = 0;
	generate_blinding_pair(A);
#endif

	return 1;

keyErr:

	if(ret)
	{
		BN_clear_free(ret);
	}
	if(pret)
	{
		Point_free(pret);
	}
	return 0;
}

int ECC_public_key_check(Point *pub, ECC *ecc)
{
	Point R;

	if(pub == NULL || ecc == NULL) goto checkErr;

	if(!ValidPoint(pub, ecc->curve)) goto checkErr;

	if(PMul(&R, pub, ecc->order, NULL, ecc->curve)) goto checkErr;

	return 1;
checkErr:
	return 0;
}


int Curve_5(ECC *ecc)
{
	Point *P=NULL;
	Curve *C=NULL;
	BIGNUM *B=NULL;

	if(ecc->base == NULL) 
	{
		if((P = Point_new()) == NULL) return 0;
		ecc->base = P;
	}

	ecc->base->x[0] = 0xbbe803cb;
	ecc->base->x[1] = 0x74880f33;
	ecc->base->x[2] = 0x329fcc3d;
	ecc->base->x[3] = 0x46103d79;
	ecc->base->x[4] = 0xaf699895;
	ecc->base->x[5] = 0x00000007;

	ecc->base->y[0] = 0xf0b7ca9f;
	ecc->base->y[1] = 0xea5848ae;
	ecc->base->y[2] = 0x1d3f87f7;
	ecc->base->y[3] = 0x5966adea;
	ecc->base->y[4] = 0xec23211b;
	ecc->base->y[5] = 0x00000001;

	ecc->cofactor = 2;

	if(ecc->curve == NULL)
	{
		if((C = Curve_new()) == NULL)
		{
			goto Kerr;
		}
		ecc->curve = C;
	}
	ecc->curve->a[0] = 0x35de5242;
	ecc->curve->a[1] = 0xf432c894;
	ecc->curve->a[2] = 0xe0789675;
	ecc->curve->a[3] = 0x5234a422;
	ecc->curve->a[4] = 0x2546b543;
	ecc->curve->a[5] = 0x00000007;
	
	ecc->curve->b[0] = 0x6f9dd4d9;
	ecc->curve->b[1] = 0x20b6cd4d;
	ecc->curve->b[2] = 0xff38c74b;
	ecc->curve->b[3] = 0xd5240d3c;
	ecc->curve->b[4] = 0xc9517d06;
	ecc->curve->b[5] = 0x00000000;

	if(ecc->order == NULL) 
	{
		if((B = BN_new()) == NULL) 
		{
			goto Kerr;
		}
		ecc->order = B;
	}
	bn_expand(ecc->order, 163);
	ecc->order->d[0] = 0x4daeafc1;
	ecc->order->d[1] = 0xc8821cc7;
	ecc->order->d[2] = 0x0001e60f;
	ecc->order->d[3] = 0x00000000;
	ecc->order->d[4] = 0x00000000;
	ecc->order->d[5] = 0x00000004;
	ecc->order->top = 6; ecc->order->neg = 0;

	ecc->priv_key = BN_new();
	ecc->pub_key = Point_new();
	return 1;
Kerr:
	if(P) Point_free(P);
	if(C) Curve_free(C);
	if(B) BN_clear_free(B);
	return 0;
}

#if 0
int Curve_K_163(ECC *ecc)
{
	Point *P=NULL;
	Curve *C=NULL;
	BIGNUM *B=NULL;

	if(ecc->base == NULL) 
	{
		if((P = Point_new()) == NULL) return 0;
		ecc->base = P;
	}

	ecc->base->x[0] = 0x5c94eee8;
	ecc->base->x[1] = 0xde4e6d5e;
	ecc->base->x[2] = 0xaa07d793;
	ecc->base->x[3] = 0x7bbc11ac;
	ecc->base->x[4] = 0xfe13c053;
	ecc->base->x[5] = 0x00000002;

	ecc->base->y[0] = 0xccdaa3d9;
	ecc->base->y[1] = 0x0536d538;
	ecc->base->y[2] = 0x321f2e80;
	ecc->base->y[3] = 0x5d38ff58;
	ecc->base->y[4] = 0x89070fb0;
	ecc->base->y[5] = 0x00000002;

	ecc->cofactor = 2;

	if(ecc->curve == NULL)
	{
		if((C = Curve_new()) == NULL)
		{
			goto Kerr;
		}
		ecc->curve = C;
	}
	ecc->curve->a[0] = 0x00000001;
	ecc->curve->a[1] = 0x00000000;
	ecc->curve->a[2] = 0x00000000;
	ecc->curve->a[3] = 0x00000000;
	ecc->curve->a[4] = 0x00000000;
	ecc->curve->a[5] = 0x00000000;
	
	ecc->curve->b[0] = 0x00000001;
	ecc->curve->b[1] = 0x00000000;
	ecc->curve->b[2] = 0x00000000;
	ecc->curve->b[3] = 0x00000000;
	ecc->curve->b[4] = 0x00000000;
	ecc->curve->b[5] = 0x00000000;

	if(ecc->order == NULL) 
	{
		if((B = BN_new()) == NULL) 
		{
			goto Kerr;
		}
		ecc->order = B;
	}
	bn_expand(ecc->order, 163);
	ecc->order->d[0] = 0x99F8A5EF;
	ecc->order->d[1] = 0xA2E0CC0D;
	ecc->order->d[2] = 0x00020108;
	ecc->order->d[3] = 0x00000000;
	ecc->order->d[4] = 0x00000000;
	ecc->order->d[5] = 0x00000004;
	ecc->order->top = 6; ecc->order->neg = 0;

	ecc->priv_key = BN_new();
	ecc->pub_key = Point_new();
	return 1;
Kerr:
	if(P) Point_free(P);
	if(C) Curve_free(C);
	if(B) BN_clear_free(B);
	return 0;
}
#endif

#if 0
void Curve_B_163(ECC *ecc)
{
	if(ecc->base == NULL) ecc->base = Point_new();

	ecc->base->x[0] = 0xe8343e36;
	ecc->base->x[1] = 0xd4994637;
	ecc->base->x[2] = 0xa0991168;
	ecc->base->x[3] = 0x86a2d57e;
	ecc->base->x[4] = 0xf0eba162;
	ecc->base->x[5] = 0x00000003;

	ecc->base->y[0] = 0x797324f1;
	ecc->base->y[1] = 0xb11c5c0c;
	ecc->base->y[2] = 0xa2cdd545;
	ecc->base->y[3] = 0x71a0094f;
	ecc->base->y[4] = 0xd51fbc6c;
	ecc->base->y[5] = 0x00000000;

	ecc->cofactor = 2;

	if(ecc->curve == NULL) ecc->curve = Curve_new();
	ecc->curve->a[0] = 0x00000001;
	ecc->curve->a[1] = 0x00000000;
	ecc->curve->a[2] = 0x00000000;
	ecc->curve->a[3] = 0x00000000;
	ecc->curve->a[4] = 0x00000000;
	ecc->curve->a[5] = 0x00000000;
	
	ecc->curve->b[0] = 0x4a3205fd;
	ecc->curve->b[1] = 0x512f7874;
	ecc->curve->b[2] = 0x1481eb10;
	ecc->curve->b[3] = 0xb8c953ca;
	ecc->curve->b[4] = 0x0a601907;
	ecc->curve->b[5] = 0x00000002;

	if(ecc->order == NULL) ecc->order = BN_new();
	BN_dec2bn(&(ecc->order), "5846006549323611672814742442876390689256843201587");

	ecc->priv_key = BN_new();
	ecc->pub_key = Point_new();
}
#endif
#if 0
int AnomalousConditionHold(ECC *ecc)
{
	BIGNUM *temp, *u;
	if(ecc == NULL) return 0;

	u = BN_new(); temp = BN_new();
	BN_set_word(temp, 1);
	BN_lshift(u, temp, 163);

	BN_copy(temp, ecc->order);
	BN_mul_word(temp, (BN_ULONG)ecc->cofactor);

	if(BN_cmp(temp, u) == 0) 
	{
		BN_clear_free(temp); BN_clear_free(u);
		return 0;
	}
	else
	{
		BN_clear_free(temp); BN_clear_free(u);
		return 1;
	}
}

int MOVConditionHold(ECC *ecc)
{
	int B = 20;
	int i;
	BIGNUM *temp, *q, *t[2];
	BN_CTX *ctx;

	temp = BN_new(); q = BN_new(); 
	t[0] = BN_new(); t[1] = BN_new();
	ctx = BN_CTX_new();

	BN_set_word(t[0], 1);
	BN_lshift(q, t[0], 163);

	for(i=1; i<=B; i++)
	{
		BN_mod_mul(t[i%2], t[(i-1)%2], q, ecc->order, ctx);
		if(BN_is_one(t[i%2]))
		{
			BN_clear_free(temp); BN_clear_free(q);
			BN_clear_free(t[0]); BN_clear_free(t[1]);
			BN_CTX_free(ctx);
			return 0;
		}
	}

	BN_clear_free(temp); BN_clear_free(q);
	BN_clear_free(t[0]); BN_clear_free(t[1]);
	BN_CTX_free(ctx);
	return 1;
}
#endif
