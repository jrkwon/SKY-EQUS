#include "Group163.h"

/*
 * Made by Suengchur Pyun!!
 * 
 * e-mail : pyun@postech.edu
 */

static Point PTab[41];
static int is_Init_PT = 0;

Point *Point_new()
{
	Point *ret;
	ret = (Point *)Malloc(sizeof(Point));
	return ret;
}

void Point_free(Point *p)
{
	if(p == NULL) return;
	memset((void *)(p->x), 0, sizeof(WORD32)*6);
	memset((void *)(p->y), 0, sizeof(WORD32)*6);
	Free(p);
}

Curve *Curve_new()
{
	Curve *ret;
	ret = (Curve *)Malloc(sizeof(Curve));
	return ret;
}

void Curve_free(Curve *c)
{
	if(c == NULL) return;
	memset((void *)(c->a), 0, sizeof(WORD32)*6);
	memset((void *)(c->b), 0, sizeof(WORD32)*6);
	Free(c);
}

static int CopyPoint(Point *B, Point *A)
{
	Copy163(B->x, A->x);
	Copy163(B->y, A->y);
	return 1;
}

/* 
 * A, B must be over Curve Cr.
 *
 * return value :
 *
 * if C is the infinite point, return 0.
 *
 * else return 1;
 */

int Eadd(Point *C, Point *A, Point *B, Curve *Cr)
{
	WORD32 temp0[6], temp1[6], temp2[6];
	WORD32 lamda[6];

#ifndef NO_INFINITE
	if(IsEqual163(A->x, B->x))
	{
		Add163(temp0, B->x, B->y);
		/* A is an inverse of B */
		if(IsEqual163(A->y, temp0)) return 0;
	}
#endif

	Add163(temp0, A->y, B->y);
	Add163(temp1, A->x, B->x);
	if(IsZero163(temp0) && IsZero163(temp1)) 
	{ 
		return Edbl(C, A, Cr);
	}
	Inv163(temp2, temp1);
	Mul163(lamda, temp2, temp0);

	Sqr163(temp0, lamda);
	Add163(temp2, temp0, lamda);
	Add163(temp0, temp1, Cr->a);
	Copy163(temp1, A->x);
	Add163(C->x, temp0, temp2);

	Add163(temp0, temp1, C->x);
	Mul163(temp1, temp0, lamda);
	Add163(temp2, C->x, A->y);
	Add163(C->y, temp1, temp2);
	return 1;
}

int Esub(Point *C, Point *A, Point *B, Curve *Cr)
{
	WORD32 temp0[6], temp1[6], temp2[6];
	WORD32 lamda[6];

#ifndef NO_INFINITE
	if(IsEqual163(A->x, B->x) && IsEqual163(A->y, B->y))
		return 0;
#endif

	Add163(temp0, A->y, B->y);
	Add163(temp0, temp0, B->x);
	Add163(temp1, A->x, B->x);
	if(IsZero163(temp0) && IsZero163(temp1)) 
	{ 
		return Edbl(C, A, Cr);
	}
	Inv163(temp2, temp1);
	Mul163(lamda, temp2, temp0);

	Sqr163(temp0, lamda);
	Add163(temp2, temp0, lamda);
	Add163(temp0, temp1, Cr->a);
	Copy163(temp1, A->x);
	Add163(C->x, temp0, temp2);

	Add163(temp0, temp1, C->x);
	Mul163(temp1, temp0, lamda);
	Add163(temp2, C->x, A->y);
	Add163(C->y, temp1, temp2);
	return 1;
}

int Edbl(Point *C, Point *A, Curve *Cr)
{
	WORD32 temp0[6], temp1[6];
	WORD32 lamda[6];

#ifndef NO_INFINITE
	if(IsZero163(A->x)) return 0;
#endif

	Inv163(temp0, A->x);
	Mul163(temp1, temp0, A->y);
	Add163(lamda, temp1, A->x);

	Sqr163(temp0, lamda);
	Add163(temp1, lamda, temp0);
	Copy163(temp0, A->x);
	Add163(C->x, temp1, Cr->a);

	Add163(temp0, temp0, C->x);
	Mul163(temp1, temp0, lamda);
	Add163(temp0, C->x, A->y);
	Add163(C->y, temp1, temp0);
	return 1;
}

int Init_PTab(Point *A, Curve *Cr)
{
	int i, j;
	Point temp;

	CopyPoint(&temp, A);
	for(i=0; i<41; i++)
	{
		CopyPoint(&PTab[i], &temp);
		for(j=0; j<4; j++) Edbl(&temp, &temp, Cr);
	}
	is_Init_PT = 1;
	return 1;
}

int PMul(Point *R, Point *P, BIGNUM *k, BIGNUM *n, Curve *Cr)
{
	BIGNUM *e=NULL, *h=NULL;
	BN_CTX *ctx=NULL;
	int i, r, bit0, bit1;

	if((ctx = BN_CTX_new()) == NULL) goto mulErr;
	if((e = BN_new()) == NULL) goto mulErr;

	if(n == NULL) 
	{
		if(!BN_copy(e, k)) goto mulErr;
	}
	else
	{
		if(!BN_mod(e, k, n, ctx)) goto mulErr;
	}

	if((h = BN_new()) == NULL) goto mulErr;

	if(!BN_copy(h, e)) goto mulErr;
	if(!BN_mul_word(h, 3)) goto mulErr;

	r = BN_num_bits(h);
	r--;

	CopyPoint(R, P);
	for(i=r-1; i>=1; i--)
	{
		if(!Edbl(R, R, Cr))
		{
			return 0;
		}
		bit0 = BN_is_bit_set(h, i);
		bit1 = BN_is_bit_set(e, i);

		if(bit0 && !bit1)
		{
			if(!Eadd(R, R, P, Cr)) return 0;
		}
		if(!bit0 && bit1)
		{
			if(!Esub(R, R, P, Cr)) return 0;
		}
	}

	BN_clear_free(e);
	BN_clear_free(h);
	BN_CTX_free(ctx);
	return 1;

mulErr:

	if(e) BN_clear_free(e);
	if(h) BN_clear_free(h);
	if(ctx) BN_CTX_free(ctx);
	return 0;
}

int BrickellMul(Point *Q, Point *P, BIGNUM *k, BIGNUM *n, Curve *Cr)
{
	BIGNUM *e=NULL;
	BN_CTX *ctx=NULL;
	int i;
	unsigned long E[41], j;
	Point B;
	int Bflag, Qflag;

	if((ctx = BN_CTX_new()) == NULL) goto brickErr;
	if((e = BN_new()) == NULL) goto brickErr;

	if(n == NULL) 
	{
		if(!BN_copy(e, k)) goto brickErr;
	}
	else
	{
		if(!BN_mod(e, k, n, ctx)) goto brickErr;
	}

	for(i=0; i<41; i++) E[i] = 0;
	for(i=0; i<e->top; i++)
	{
		for(j=0; j<8 && i*8+j<41; j++) E[i*8+j] = (e->d[i]>>(j*4))&0xf;
	}

	Bflag = 0; Qflag = 0;
	if(!is_Init_PT) Init_PTab(P, Cr);

	for(j=15; j>=1; j--)
	{
		for(i=0; i<41; i++) 
		{
			if(E[i] == j) 
			{
				if(Bflag) 
				{
					if(!Eadd(&B, &B, &PTab[i], Cr)) return 0;
				}
				else 
				{
					CopyPoint(&B, &PTab[i]);
					Bflag = 1;
				}
			}
		}
		if(Qflag) Eadd(Q, &B, Q, Cr);
		else 
		{
			if(Bflag)
			{
				CopyPoint(Q, &B);
				Qflag = 1;
			}
		}
	}

	BN_clear_free(e);
	BN_CTX_free(ctx);
	return 1;

brickErr:

	if(e) BN_clear_free(e);
	if(ctx) BN_CTX_free(ctx);
	return 0;
}

int ValidPoint(Point *A, Curve *Cr)
{
	WORD32 temp0[6], temp1[6], temp2[6];
	WORD32 temp3[6];

	Sqr163(temp0, A->y);
	Mul163(temp1, A->x, A->y);
	Add163(temp0, temp0, temp1);

	Sqr163(temp1, A->x);
	Add163(temp2, A->x, Cr->a);
	Mul163(temp3, temp1, temp2);
	Add163(temp1, temp3, Cr->b);

	if(IsEqual163(temp0, temp1)) return 1;
	return 0;
}

int MakePoint(Point *C, Curve *Cr)
{
	int i;
	WORD32 temp0[6], temp1[6], beta[6];
	if(C == NULL || Cr == NULL)
	{
		return 0;
	}

/* 
 * If X_p == 0, then Y_p = b^(2^(m-1))
 */
	if(IsZero163(C->x)) 
 	{
		Copy163(C->y, Cr->b);
		for(i=0; i<162; i++) Sqr163(C->y, C->y);
		return 1;
	}

	while(1)
	{
/*
 * Calculate Beta Value 
 * Beta = X_p + a + b*(X_p)^(-2)
 */
		Inv163(temp0, C->x);
		Sqr163(temp0, temp0);
		Mul163(temp1, temp0, Cr->b);
		Add163(temp1, temp1, Cr->a);
		Add163(beta, temp1, C->x);

		Copy163(temp0, beta);
		for(i=1; i<=81; i++)
		{
			Sqr163(temp0, temp0);
			Sqr163(temp0, temp0);
			Add163(temp0, temp0, beta);
		}

		Sqr163(temp1, temp0);
		Add163(temp1, temp1, temp0);

		if(IsEqual163(temp1, beta)) break;
		C->x[4]++;
	}
	Mul163(C->y, C->x, temp0);
	return 1;
}

int OctetSizeOfPoint(int form)
{
	int ol;
	
	ol = (163+7)/8;

	if(!form) return ol*2+1;
	else return ol+2;
}


unsigned char *Point2Octet(Point *A, int form)
{
	int ol, fsize;
	unsigned char *B;

	fsize = 163;

	ol = (fsize+7)/8;

	switch(form)
	{
	/* uncompressed form */
	case 0 :
		if((B=(unsigned char *)Malloc(2*ol+1))==NULL) return NULL;
		B[0] = 0x04;
		Field2Octet(&B[1], A->x, fsize);
		Field2Octet(&B[1+ol], A->y, fsize);
		return B;
	/* compressed form */
	case 1 :
		break;
	/* hybrid form */
	case 2 :
		break;
	default : break;
	}
	return NULL;
}

int Octet2Point(Point *A, unsigned char *B, int llen)
{
	int octlen;
	int ret = 0;
	unsigned char *Xoctet=NULL, *Yoctet=NULL;

	/* uncompressed form */
	if(B[0] == 0x04)
	{
		octlen = llen/2;
		if((Xoctet=(unsigned char *)Malloc(octlen))==NULL) goto o2pErr;
		if((Yoctet=(unsigned char *)Malloc(octlen))==NULL) goto o2pErr;
		memcpy(Xoctet, &B[1], octlen);
		Octet2Field(A->x, Xoctet, octlen);
		memcpy(Yoctet, &B[1+octlen], octlen);
		Octet2Field(A->y, Yoctet, octlen);
		ret = 2*octlen+1;
	}

o2pErr:
	if(Xoctet) Free(Xoctet);
	if(Yoctet) Free(Yoctet);
	return ret;
}
