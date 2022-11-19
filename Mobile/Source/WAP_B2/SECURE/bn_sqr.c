#include "bn_lcl.h"

int BN_Sqr(r, a, ctx)
BIGNUM *r;
BIGNUM *a;
BN_CTX *ctx;
	{
	int i,j,max,al;
	BIGNUM *tmp;
	BN_ULONG *ap,*rp,c;

	tmp=ctx->bn[ctx->tos];

	al=a->top;
	if (al == 0)
		{
		r->top=0;
		return(1);
		}

	max=(al*2);
	if (bn_expand(r,max*BN_BITS2) == NULL) return(0);
	if (bn_expand(tmp,max*BN_BITS2) == NULL) return(0);

	r->neg=0;

	ap=a->d;
	rp=r->d;
	rp[0]=rp[max-1]=0;
	rp++;
	j=al;

	if (--j > 0)
		{
		ap++;
		rp[j]=bn_mul_word(rp,ap,j,ap[-1]);
		rp+=2;
		}

	for (i=2; i<al; i++)
		{
		j--;
		ap++;
		rp[j]=bn_mul_add_word(rp,ap,j,ap[-1]);
		rp+=2;
		}

	j=max;
	rp=r->d;
	c=0;
	for (i=0; i<j; i++)
		{
		BN_ULONG t;

		t= *rp;
		*(rp++)=((t<<1)|c)&BN_MASK2;
		c=(t & BN_TBIT)?1:0;
		}

	bn_sqr_words(tmp->d,a->d,al);

	ap=tmp->d;
	rp=r->d;
	c=0;
	j=max;
	for (i=0; i<j; i++)
		{
		BN_ULONG t1,t2;

		t1= *(ap++);
		t2= *rp;
		if (c)
			{
			c=(t2 >= ((~t1)&BN_MASK2));
			t2=(t1+t2+1)&BN_MASK2;
			}
		else
			{
			t2=(t1+t2)&BN_MASK2;
			c=(t2<t1);
			}
		*(rp++)=t2;
		}

	r->top=max;
	if (r->d[max-1] == 0) r->top--;
	return(1);
	}

