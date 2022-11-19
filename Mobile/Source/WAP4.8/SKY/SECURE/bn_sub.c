
#include <string.h>
#include "bn_lcl.h"

void bn_qsub(r, a, b)
BIGNUM *r;
BIGNUM *a;
BIGNUM *b;
	{
	int max,min;
	register BN_ULONG t1,t2,*ap,*bp,*rp;
	int i,carry;

	max=a->top;
	min=b->top;
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
			carry=(t1 <= t2);
			t1=(t1-t2-1);
			}
		else
			{
			carry=(t1 < t2);
			t1=(t1-t2);
			}
		*(rp++)=t1&BN_MASK2;
		}
	if (carry)
		{
		while (i < max)
			{
			i++;
			t1= *(ap++);
			t2=(t1-1)&BN_MASK2;
			*(rp++)=t2;
			if (t1 > t2) break;
			}
		}
	memcpy(rp,ap,sizeof(*rp)*(max-i));

	r->top=max;
	bn_fix_top(r);
	}

int BN_Sub(r, a, b)
BIGNUM *r;
BIGNUM *a;
BIGNUM *b;
	{
	int max,i;
	int add=0,neg=0;
	BIGNUM *tmp;

	if (a->neg)
		{
		if (b->neg)
			{ tmp=a; a=b; b=tmp; }
		else
			{ add=1; neg=1; }
		}
	else
		{
		if (b->neg) { add=1; neg=0; }
		}

	if (add)
		{
		i=(a->top > b->top);
	        if (bn_expand_wap(r,(((i)?a->top:b->top)+1)*BN_BITS2) == NULL)
			return(0);
		if (i)
			bn_qadd(r,a,b);
		else
			bn_qadd(r,b,a);
		r->neg=neg;
		return(1);
		}

	max=(a->top > b->top)?a->top:b->top;
	if (bn_expand_wap(r,max*BN_BITS2) == NULL) return(0);
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

