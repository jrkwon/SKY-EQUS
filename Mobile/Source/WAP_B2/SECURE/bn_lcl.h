#ifndef HEADER_BN_LCL_H
#define HEADER_BN_LCL_H

#include "bn.h"

#define NULL 0
#define Lw(t)    (((BN_ULONG)(t))&BN_MASK2)
#define Hw(t)    (((BN_ULONG)((t)>>BN_BITS2))&BN_MASK2)

#define bn_expand(n,b) ((((b)/BN_BITS2) <= (n)->max)?(n):bn_expand2((n),(b)))

#ifdef BN_LLONG
#define mul_add(r,a,w,c) { \
	BN_ULLONG t; \
	t=(BN_ULLONG)w * (a) + (r) + (c); \
	(r)=Lw(t); \
	(c)= Hw(t); \
	}

#define mul(r,a,w,c) { \
	BN_ULLONG t; \
	t=(BN_ULLONG)w * (a) + (c); \
	(r)=Lw(t); \
	(c)= Hw(t); \
	}

#define bn_mul_words(r1,r2,a,b) \
	{ \
	BN_ULLONG t; \
	t=(BN_ULLONG)(a)*(b); \
	r1=Lw(t); \
	r2=Hw(t); \
	}

#else

#define LBITS(a)	((a)&BN_MASK2l)
#define HBITS(a)	(((a)>>BN_BITS4)&BN_MASK2l)
#define	L2HBITS(a)	((BN_ULONG)((a)&BN_MASK2l)<<BN_BITS4)

#define LLBITS(a)	((a)&BN_MASKl)
#define LHBITS(a)	(((a)>>BN_BITS2)&BN_MASKl)
#define	LL2HBITS(a)	((BN_ULLONG)((a)&BN_MASKl)<<BN_BITS2)

#define mul64(l,h,bl,bh) \
	{ \
	BN_ULONG m,m1,lt,ht; \
 \
	lt=l; \
	ht=h; \
	m =(bh)*(lt); \
	lt=(bl)*(lt); \
	m1=(bl)*(ht); \
	ht =(bh)*(ht); \
	m+=m1; if ((m&BN_MASK2) < m1) ht+=L2HBITS(1L); \
	ht+=HBITS(m); \
	m1=L2HBITS(m); \
	lt+=m1; if ((lt&BN_MASK2) < m1) ht++; \
	(l)=lt; \
	(h)=ht; \
	}

#define sqr64(lo,ho,in) \
	{ \
	BN_ULONG l,h,m; \
 \
	h=(in); \
	l=LBITS(h); \
	h=HBITS(h); \
	m =(l)*(h); \
	l*=l; \
	h*=h; \
	h+=(m&BN_MASK2h1)>>(BN_BITS4-1); \
	m =(m&BN_MASK2l)<<(BN_BITS4+1); \
	l+=m; if ((l&BN_MASK2) < m) h++; \
	(lo)=l; \
	(ho)=h; \
	}

#define mul_add(r,a,bl,bh,c) { \
	BN_ULONG l,h; \
 \
	h= (a); \
	l=LBITS(h); \
	h=HBITS(h); \
	mul64(l,h,(bl),(bh)); \
 \
	l+=(c); if ((l&BN_MASK2) < (c)) h++; \
	(c)=(r); \
	l+=(c); if ((l&BN_MASK2) < (c)) h++; \
	(c)=h&BN_MASK2; \
	(r)=l&BN_MASK2; \
	}

#define mul(r,a,bl,bh,c) { \
	BN_ULONG l,h; \
 \
	h= (a); \
	l=LBITS(h); \
	h=HBITS(h); \
	mul64(l,h,(bl),(bh)); \
 \
	l+=(c); if ((l&BN_MASK2) < (c)) h++; \
	(c)=h&BN_MASK2; \
	(r)=l&BN_MASK2; \
	}

#define bn_mul_words(r1,r2,a,b) \
	{ \
	BN_ULONG l,h,bl,bh; \
 \
	h=(a); \
	l=LBITS(h); \
	h=HBITS(h); \
	bh=(b); \
	bl=LBITS(bh); \
	bh=HBITS(bh); \
 \
	mul64(l,h,bl,bh); \
 \
	(r1)=l; \
	(r2)=h; \
	}
#endif

BIGNUM *bn_expand2(BIGNUM *b, int bits);


#endif
