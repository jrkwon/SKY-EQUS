
#include "bn_lcl.h"

int BN_Mul(r, a, b)
BIGNUM *r;
BIGNUM *a;
BIGNUM *b;
	{
	int i;
	int max,al,bl;
	BN_ULONG *ap,*bp,*rp;

	al=a->top;
	bl=b->top;
	if ((al == 0) || (bl == 0))
		{
		r->top=0;
		return(1);
		}

	max=(al+bl);
	if (bn_expand_wap(r,(max)*BN_BITS2) == NULL) return(0);

	r->top=max;

	r->neg=a->neg^b->neg;

	ap=a->d;
	bp=b->d;
	rp=r->d;

	rp[al]=bn_mul_word(rp,ap,al,*(bp++));
	rp++;
	for (i=1; i<bl; i++)
		{
		rp[al]=bn_mul_add_word(rp,ap,al,*(bp++));
		rp++;
		}
	if (r->d[max-1] == 0) r->top--;
	return(1);
	}

