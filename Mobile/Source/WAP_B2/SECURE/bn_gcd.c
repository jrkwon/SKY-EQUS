#include "bn_lcl.h"
#include "comdef.h"

#ifndef NOPROTO
static BIGNUM *euclid(BIGNUM *a, BIGNUM *b);
#else
static BIGNUM *euclid();
#endif

static BIGNUM *euclid(a,b)
BIGNUM *a,*b;
	{
	BIGNUM *t;
	int shifts=0;

	for (;;)
		{
		if (BN_IsZero(b))
			break;

		if (BN_IsOdd(a))
			{
			if (BN_IsOdd(b))
				{
				if (!BN_Sub(a,a,b)) goto err;
				if (!BN_Rshift1(a,a)) goto err;
				if (BN_Cmp(a,b) < 0)
					{ t=a; a=b; b=t; }
				}
			else
				{
				if (!BN_Rshift1(b,b)) goto err;
				if (BN_Cmp(a,b) < 0)
					{ t=a; a=b; b=t; }
				}
			}
		else	
			{
			if (BN_IsOdd(b))
				{
				if (!BN_Rshift1(a,a)) goto err;
				if (BN_Cmp(a,b) < 0)
					{ t=a; a=b; b=t; }
				}
			else
				{
				if (!BN_Rshift1(a,a)) goto err;
				if (!BN_Rshift1(b,b)) goto err;
				shifts++;
				}
			}
		}
	if (shifts)
		{
		if (!BN_Lshift(a,a,shifts)) goto err;
		}
	return(a);
err:
	return(NULL);
	}

BIGNUM *BN_ModInverse(a, n, ctx)
BIGNUM *a;
BIGNUM *n;
BN_CTX *ctx;
	{
	BIGNUM *A,*B,*X,*Y,*M,*D,*R;
	BIGNUM *ret=NULL,*T;
	int sign;

	A=ctx->bn[ctx->tos];
	B=ctx->bn[ctx->tos+1];
	X=ctx->bn[ctx->tos+2];
	D=ctx->bn[ctx->tos+3];
	M=ctx->bn[ctx->tos+4];
	Y=ctx->bn[ctx->tos+5];
	ctx->tos+=6;
	R=BN_New();
	if (R == NULL) goto err;

	BN_Zero(X);
	BN_One(Y);
	if (BN_Copy(A,a) == NULL) goto err;
	if (BN_Copy(B,n) == NULL) goto err;
	sign=1;

	while (!BN_IsZero(B))
		{
		if (!BN_Div(D,M,A,B,ctx)) goto err;
		T=A;
		A=B;
		B=M;

		if (!BN_Mul(T,D,X)) goto err;
		if (!BN_Add(T,T,Y)) goto err;
		M=Y;
		Y=X;
		X=T;
		sign= -sign;
		}
	if (sign < 0)
		{
		if (!BN_Sub(Y,n,Y)) goto err;
		}

	if (BN_IsOne(A))
		{ if (!BN_Div(NULL,R,Y,n,ctx)) goto err; }
	else
		{
		goto err;
		}
	ret=R;
err:
	if ((ret == NULL) && (R != NULL)) BN_Free(R);
	ctx->tos-=6;
	return(ret);
	}

