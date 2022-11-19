#include "bn_lcl.h"
#include "aapiclnt.h"

#ifdef BN_LLONG 
BN_ULONG bn_mul_add_word(rp,ap,num,w)
BN_ULONG *rp,*ap;
int num;
BN_ULONG w;
	{
	BN_ULONG c1=0;

	for (;;)
		{
		mul_add(rp[0],ap[0],w,c1);
		if (--num == 0) break;
		mul_add(rp[1],ap[1],w,c1);
		if (--num == 0) break;
		mul_add(rp[2],ap[2],w,c1);
		if (--num == 0) break;
		mul_add(rp[3],ap[3],w,c1);
		if (--num == 0) break;
		ap+=4;
		rp+=4;
		}
	return(c1);


	} 

BN_ULONG bn_mul_word(rp,ap,num,w)
BN_ULONG *rp,*ap;
int num;
BN_ULONG w;
	{
	BN_ULONG c1=0;

	for (;;)
		{
		mul(rp[0],ap[0],w,c1);
		if (--num == 0) break;
		mul(rp[1],ap[1],w,c1);
		if (--num == 0) break;
		mul(rp[2],ap[2],w,c1);
		if (--num == 0) break;
		mul(rp[3],ap[3],w,c1);
		if (--num == 0) break;
		ap+=4;
		rp+=4;
		}
	return(c1);
	} 

void bn_sqr_words(r,a,n)
BN_ULONG *r,*a;
int n;
        {
	for (;;)
		{
		BN_ULLONG t;

		t=(BN_ULLONG)(a[0])*(a[0]);
		r[0]=Lw(t); r[1]=Hw(t);
		if (--n == 0) break;

		t=(BN_ULLONG)(a[1])*(a[1]);
		r[2]=Lw(t); r[3]=Hw(t);
		if (--n == 0) break;

		t=(BN_ULLONG)(a[2])*(a[2]);
		r[4]=Lw(t); r[5]=Hw(t);
		if (--n == 0) break;

		t=(BN_ULLONG)(a[3])*(a[3]);
		r[6]=Lw(t); r[7]=Hw(t);
		if (--n == 0) break;

		a+=4;
		r+=8;
		}
	}

#else

BN_ULONG bn_mul_add_word(rp,ap,num,w)
BN_ULONG *rp,*ap;
int num;
BN_ULONG w;
	{
	BN_ULONG c=0;
	BN_ULONG bl,bh;

	bl=LBITS(w);
	bh=HBITS(w);

	for (;;)
		{
		mul_add(rp[0],ap[0],bl,bh,c);
		if (--num == 0) break;
		mul_add(rp[1],ap[1],bl,bh,c);
		if (--num == 0) break;
		mul_add(rp[2],ap[2],bl,bh,c);
		if (--num == 0) break;
		mul_add(rp[3],ap[3],bl,bh,c);
		if (--num == 0) break;
		ap+=4;
		rp+=4;
		}
	return(c);
	} 

BN_ULONG bn_mul_word(rp,ap,num,w)
BN_ULONG *rp,*ap;
int num;
BN_ULONG w;
	{
	BN_ULONG carry=0;
	BN_ULONG bl,bh;

	bl=LBITS(w);
	bh=HBITS(w);

	for (;;)
		{
		mul(rp[0],ap[0],bl,bh,carry);
		if (--num == 0) break;
		mul(rp[1],ap[1],bl,bh,carry);
		if (--num == 0) break;
		mul(rp[2],ap[2],bl,bh,carry);
		if (--num == 0) break;
		mul(rp[3],ap[3],bl,bh,carry);
		if (--num == 0) break;
		ap+=4;
		rp+=4;
		}
	return(carry);
	} 

void bn_sqr_words(r,a,n)
BN_ULONG *r,*a;
int n;
        {
	for (;;)
		{
		sqr64(r[0],r[1],a[0]);
		if (--n == 0) break;

		sqr64(r[2],r[3],a[1]);
		if (--n == 0) break;

		sqr64(r[4],r[5],a[2]);
		if (--n == 0) break;

		sqr64(r[6],r[7],a[3]);
		if (--n == 0) break;

		a+=4;
		r+=8;
		}
	}

#endif

#if defined(BN_LLONG) && defined(BN_DIV2W)

BN_ULONG bn_div64(h,l,d)
BN_ULONG h,l,d;
	{
	return((BN_ULONG)(((((BN_ULLONG)h)<<BN_BITS2)|l)/(BN_ULLONG)d));
	}

#else

BN_ULONG bn_div64(h,l,d)
BN_ULONG h,l,d;
	{
	BN_ULONG dh,dl,q,ret=0,th,tl,t;
	int i,count=2;

	if (d == 0) return(BN_MASK2);

	i=BN_NumBitsWord(d);
	if ((i != BN_BITS2) && (h > (BN_ULONG)1<<i))
		{
				//abort();
		}
	i=BN_BITS2-i;
	if (h >= d) h-=d;

	if (i)
		{
		d<<=i;
		h=(h<<i)|(l>>(BN_BITS2-i));
		l<<=i;
		}
	dh=(d&BN_MASK2h)>>BN_BITS4;
	dl=(d&BN_MASK2l);
	for (;;)
		{
		if ((h>>BN_BITS4) == dh)
			q=BN_MASK2l;
		else
			q=h/dh;

		for (;;)
			{
			t=(h-q*dh);
			if ((t&BN_MASK2h) ||
				((dl*q) <= (
					(t<<BN_BITS4)+
					((l&BN_MASK2h)>>BN_BITS4))))
				break;
			q--;
			}
		th=q*dh;
		tl=q*dl;
		t=(tl>>BN_BITS4);
		tl=(tl<<BN_BITS4)&BN_MASK2h;
		th+=t;

		if (l < tl) th++;
		l-=tl;
		if (h < th)
			{
			h+=d;
			q--;
			}
		h-=th;

		if (--count == 0) break;

		ret=q<<BN_BITS4;
		h=((h<<BN_BITS4)|(l>>BN_BITS4))&BN_MASK2;
		l=(l&BN_MASK2l)<<BN_BITS4;
		}
	ret|=q;
	return(ret);
	}
#endif

