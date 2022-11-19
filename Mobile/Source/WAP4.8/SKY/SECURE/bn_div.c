#include "comdef.h"
#include "bn_lcl.h"
#include "aapiclnt.h"

int BN_Div(dv, rm, num, divisor,ctx)
BIGNUM *dv;
BIGNUM *rm;
BIGNUM *num;
BIGNUM *divisor;
BN_CTX *ctx;
{
	int norm_shift,i,j,loop;
	BIGNUM *tmp,wnum,*snum,*sdiv,*res;
	BN_ULONG *resp,*wnump;
	BN_ULONG d0,d1;
	int num_n,div_n;

	if (BN_IsZero(num))
		{
		return(0);
		}

	if (BN_Ucmp(num,divisor) < 0)
		{
		if (rm != NULL)
			{ if (BN_Copy(rm,num) == NULL) return(0); }
		if (dv != NULL) BN_Zero(dv);
		return(1);
		}

	tmp=ctx->bn[ctx->tos]; 
	tmp->neg=0;
	snum=ctx->bn[ctx->tos+1];
	sdiv=ctx->bn[ctx->tos+2];
	if (dv == NULL)
		res=ctx->bn[ctx->tos+3];
	else	res=dv;

	norm_shift=BN_BITS2-((BN_NumBits(divisor))%BN_BITS2);
	BN_Lshift(sdiv,divisor,norm_shift);
	sdiv->neg=0;
	norm_shift+=BN_BITS2;
	BN_Lshift(snum,num,norm_shift);
	snum->neg=0;
	div_n=sdiv->top;
	num_n=snum->top;
	loop=num_n-div_n;

	wnum.d=	 &(snum->d[loop]);
	wnum.top= div_n;
	wnum.max= snum->max;
	wnum.neg= 0;

	d0=sdiv->d[div_n-1];
	d1=(div_n == 1)?0:sdiv->d[div_n-2];

	wnump= &(snum->d[num_n-1]);

	res->neg= (num->neg^divisor->neg);
	res->top=loop;
	if (!bn_expand_wap(res,(loop+1)*BN_BITS2)) goto err;
	resp= &(res->d[loop-1]);

	if (!bn_expand_wap(tmp,(div_n+1)*BN_BITS2)) goto err;

	if (BN_Ucmp(&wnum,sdiv) >= 0)
		{
		bn_qsub(&wnum,&wnum,sdiv);
		*resp=1;
		res->d[res->top-1]=1;
		}
	else
		res->top--;
	resp--;

	for (i=0; i<loop-1; i++)
		{
		BN_ULONG q,n0,n1;
		BN_ULONG l0;

		wnum.d--; wnum.top++;
		n0=wnump[0];
		n1=wnump[-1];
		if (n0 == d0)
			q=BN_MASK2;
		else
			q=bn_div64(n0,n1,d0);
		{
#ifdef BN_LLONG
		BN_ULLONG t1,t2,rem;
		t1=((BN_ULLONG)n0<<BN_BITS2)|n1;
		for (;;)
			{
			t2=(BN_ULLONG)d1*q;
			rem=t1-(BN_ULLONG)q*d0;
			if ((rem>>BN_BITS2) ||
				(t2 <= ((BN_ULLONG)(rem<<BN_BITS2)+wnump[-2])))
				break;
			q--;
			}
#else
		BN_ULONG t1l,t1h,t2l,t2h,t3l,t3h,ql,qh,t3t;
		t1h=n0;
		t1l=n1;
		for (;;)
			{
			t2l=LBITS(d1); t2h=HBITS(d1);
			ql =LBITS(q);  qh =HBITS(q);
			mul64(t2l,t2h,ql,qh);

			t3t=LBITS(d0); t3h=HBITS(d0);
			mul64(t3t,t3h,ql,qh);
			t3l=(t1l-t3t);
			if (t3l > t1l) t3h++;
			t3h=(t1h-t3h);

			if (t3h) break;
			if (t2h < t3l) break;
			if ((t2h == t3l) && (t2l <= wnump[-2])) break;

			q--;
			}
#endif
		}
		l0=bn_mul_word(tmp->d,sdiv->d,div_n,q);
		tmp->d[div_n]=l0;
		for (j=div_n+1; j>0; j--)
			if (tmp->d[j-1]) break;
		tmp->top=j;

		j=wnum.top;
		BN_Sub(&wnum,&wnum,tmp);

		snum->top=snum->top+wnum.top-j;

		if (wnum.neg)
			{
			q--;
			j=wnum.top;
			BN_Add(&wnum,&wnum,sdiv);
			snum->top+=wnum.top-j;
			}
		*(resp--)=q;
		wnump--;
		}
	if (rm != NULL)
		{
		BN_Rshift(rm,snum,norm_shift);
		rm->neg=num->neg;
		}
	return(1);
err:
	return(0);
}
