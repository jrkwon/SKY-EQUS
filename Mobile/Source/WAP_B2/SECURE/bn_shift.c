#include <string.h>
#include "bn_lcl.h"

int BN_Lshift1(r, a)
BIGNUM *r;
BIGNUM *a;
	{
	register BN_ULONG *ap,*rp,t,c;
	int i;

	if (r != a)
		{
		r->neg=a->neg;
		if (bn_expand(r,(a->top+1)*BN_BITS2) == NULL) return(0);
		r->top=a->top;
		}
	else
		{
		if (bn_expand(r,(a->top+1)*BN_BITS2) == NULL) return(0);
		}
	ap=a->d;
	rp=r->d;
	c=0;
	for (i=0; i<a->top; i++)
		{
		t= *(ap++);
		*(rp++)=((t<<1)|c)&BN_MASK2;
		c=(t & BN_TBIT)?1:0;
		}
	if (c)
		{
		*rp=1;
		r->top++;
		}
	return(1);
	}

int BN_Rshift1(r, a)
BIGNUM *r;
BIGNUM *a;
	{
	BN_ULONG *ap,*rp,t,c;
	int i;

	if (BN_IsZero(a))
		{
		BN_Zero(r);
		return(1);
		}
	if (a != r)
		{
		if (bn_expand(r,a->top*BN_BITS2) == NULL) return(0);
		r->top=a->top;
		r->neg=a->neg;
		}
	ap=a->d;
	rp=r->d;
	c=0;
	for (i=a->top-1; i>=0; i--)
		{
		t=ap[i];
		rp[i]=((t>>1)&BN_MASK2)|c;
		c=(t&1)?BN_TBIT:0;
		}
	bn_fix_top(r);
	return(1);
	}

int BN_Lshift(r, a, n)
BIGNUM *r;
BIGNUM *a;
int n;
	{
	int i,nw,lb,rb;
	BN_ULONG *t,*f;
	BN_ULONG l;

	r->neg=a->neg;
	if (bn_expand(r,(a->top*BN_BITS2)+n) == NULL) return(0);
	nw=n/BN_BITS2;
	lb=n%BN_BITS2;
	rb=BN_BITS2-lb;
	f=a->d;
	t=r->d;
	t[a->top+nw]=0;
	if (lb == 0)
		for (i=a->top-1; i>=0; i--)
			t[nw+i]=f[i];
	else
		for (i=a->top-1; i>=0; i--)
			{
			l=f[i];
			t[nw+i+1]|=(l>>rb)&BN_MASK2;
			t[nw+i]=(l<<lb)&BN_MASK2;
			}
	memset(t,0,nw*sizeof(t[0]));
	r->top=a->top+nw+1;
	bn_fix_top(r);
	return(1);
	}

int BN_Rshift(r, a, n)
BIGNUM *r;
BIGNUM *a;
int n;
	{
	int i,j,nw,lb,rb;
	BN_ULONG *t,*f;
	BN_ULONG l,tmp;

	nw=n/BN_BITS2;
	rb=n%BN_BITS2;
	lb=BN_BITS2-rb;
	if (nw > a->top)
		{
		BN_Zero(r);
		return(1);
		}
	if (r != a)
		{
		r->neg=a->neg;
		if (bn_expand(r,(a->top-nw+1)*BN_BITS2) == NULL) return(0);
		}

	f= &(a->d[nw]);
	t=r->d;
	j=a->top-nw;
	r->top=j;

	if (rb == 0)
		{
		for (i=j+1; i > 0; i--)
			*(t++)= *(f++);
		}
	else
		{
		l= *(f++);
		for (i=1; i<j; i++)
			{
			tmp =(l>>rb)&BN_MASK2;
			l= *(f++);
			*(t++) =(tmp|(l<<lb))&BN_MASK2;
			}
		*(t++) =(l>>rb)&BN_MASK2;
		}
	*t=0;
	bn_fix_top(r);
	return(1);
	}
