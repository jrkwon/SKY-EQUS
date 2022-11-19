#include "bn_lcl.h"
#include "comdef.h"

int BN_Add(r, a, b)
BIGNUM *r;
BIGNUM *a;
BIGNUM *b;
{
	int i;
	BIGNUM *tmp;

	if (a->neg ^ b->neg) {
		
		if (a->neg) {
			tmp=a; a=b; b=tmp;
		}

		if (bn_expand(r,((a->top > b->top)?a->top:b->top)*BN_BITS2)
			== NULL) return(0);

		if (BN_Ucmp(a,b) < 0)
			{
			bn_qsub(r,b,a);
			r->neg=1;
			}
		else
			{
			bn_qsub(r,a,b);
			r->neg=0;
			}
		return(1);
		}

	if (a->neg)
		r->neg=1;
	else
		r->neg=0;

	i=(a->top > b->top);
	if (bn_expand(r,(((i)?a->top:b->top)+1)*BN_BITS2) == NULL) return(0);

	if (i)
		bn_qadd(r,a,b);
	else
		bn_qadd(r,b,a);
	return(1);
}

void bn_qadd(r,a,b)
BIGNUM *r;
BIGNUM *a;
BIGNUM *b;
{
	register int i;
	int max,min;
	BN_ULONG *ap,*bp,*rp,carry,t1,t2;

	max=a->top;
	min=b->top;
	r->top=max;

	ap=a->d;
	bp=b->d;
	rp=r->d;
	carry=0;
	for (i=0; i<min; i++)
		{
		t1= *(ap++);
		t2= *(bp++);
		if (carry)
			{
			carry=(t2 >= ((~t1)&BN_MASK2));
			t2=(t1+t2+1)&BN_MASK2;
			}
		else
			{
			t2=(t1+t2)&BN_MASK2;
			carry=(t2 < t1);
			}
		*(rp++)=t2;
		}
	if (carry)
		{
		while (i < max)
			{
			t1= *(ap++);
			t2=(t1+1)&BN_MASK2;
			*(rp++)=t2;
			carry=(t2 < t1);
			i++;
			if (!carry) break;
			}
		if ((i >= max) && carry)
			{
			*(rp++)=1;
			r->top++;
			}
		}
	for (; i<max; i++)
		*(rp++)= *(ap++);
}

