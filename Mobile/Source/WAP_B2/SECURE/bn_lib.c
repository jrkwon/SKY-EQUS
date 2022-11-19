#include <string.h>
#include "bn_lcl.h"

extern void  wip_free(void* mem);
extern void *wip_malloc(unsigned long size);

BIGNUM *BN_ValueOne()
	{
	static BN_ULONG data_one=1L;
	static BIGNUM const_one={&data_one,1,1,0};

	return(&const_one);
	}

int BN_NumBitsWord(l)
BN_ULONG l;
{
	static char bits[256]={
		0,1,2,2,3,3,3,3,4,4,4,4,4,4,4,4,
		5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
		6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
		6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
		7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
		7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
		7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
		7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
		8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
		8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
		8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
		8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
		8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
		8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
		8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
		8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
		};

#if defined(THIRTY_TWO_BIT) || defined(SIXTY_FOUR_BIT) || defined(SIXTY_FOUR_BIT_LONG)
		if (l & 0xffff0000L) {
			if (l & 0xff000000L)
				return(bits[l>>24L]+24);
			else	return(bits[l>>16L]+16);
		}
		else {
#endif
#if defined(SIXTEEN_BIT) || defined(THIRTY_TWO_BIT) || defined(SIXTY_FOUR_BIT) || defined(SIXTY_FOUR_BIT_LONG)
			if (l & 0xff00L)
				return(bits[l>>8]+8);
			else	
#endif
				return(bits[l   ]  );
		}
}

int BN_NumBits(a)
BIGNUM *a;
{
	BN_ULONG l;
	int i;

	if (a->top == 0) return(0);
	l=a->d[a->top-1];
	i=(a->top-1)*BN_BITS2;
	if (l == 0) {
		//abort();
	}
	return(i+BN_NumBitsWord(l));
}

void BN_ClearFree(a)
BIGNUM *a;
{
	if (a == NULL) return;
	if (a->d != NULL) {
		memset(a->d,0,a->max*sizeof(a->d[0]));
		wip_free(a->d);
	}
	memset(a,0,sizeof(BIGNUM));
	wip_free(a);
}

void BN_Free(a)
BIGNUM *a;
{
	if (a == NULL) return;
	if (a->d != NULL) wip_free(a->d);
	wip_free(a);
}

BIGNUM *BN_New()
{
	BIGNUM *ret;
	BN_ULONG *p;

	ret=(BIGNUM *)wip_malloc(sizeof(BIGNUM));
	if (ret == NULL) return(NULL);
	ret->top=0;
	ret->neg=0;
	ret->max=(BN_DEFAULT_BITS/BN_BITS2);
	p=(BN_ULONG *)wip_malloc(sizeof(BN_ULONG)*(ret->max+1));
	if (p == NULL) return(NULL);
	ret->d=p;

	memset(p,0,(ret->max+1)*sizeof(p[0]));
	return(ret);
}

BN_CTX *BN_CTX_New()
{
	BN_CTX *ret;
	BIGNUM *n;
	int i,j;

	ret=(BN_CTX *)wip_malloc(sizeof(BN_CTX));
	if (ret == NULL) return NULL;

	for (i=0; i<BN_CTX_NUM; i++) {
		n=BN_New();
		if (n == NULL) {
			for (j=0; j<i; j++)
				BN_Free(ret->bn[j]);
			wip_free(ret);
			return(NULL);
		}
		ret->bn[i]=n;
	}

	ret->bn[BN_CTX_NUM]=NULL;

	ret->tos=0;
	return(ret);
}

void BN_CTX_Free(c)
BN_CTX *c;
{
	int i;

	for (i=0; i<BN_CTX_NUM; i++)
		BN_ClearFree(c->bn[i]);
	wip_free(c);
}

BIGNUM *bn_expand2(BIGNUM *b, int bits)
{
	BN_ULONG *p;
	register int n;

//jjkyg 2000/02/24===============================================
	BN_ULONG* ret;
//===============================================================

	while (bits > b->max*BN_BITS2)
	{
		n=(bits+BN_BITS2-1)/BN_BITS2;
/*===============================================================
			realloc()을 대처하기 위함
			  jjkyg 2000/02/24
 ===============================================================*/
		ret = (BN_ULONG *)wip_malloc(sizeof(BN_ULONG)*(b->max));
		if(ret == NULL) return NULL;

		memcpy(ret,b->d,(b->max)*sizeof(BN_ULONG));
		wip_free(b->d);

		b->d = (BN_ULONG *)wip_malloc(sizeof(BN_ULONG)*(n+1));
		if(b->d == NULL) return NULL;

		memcpy(b->d,ret,(b->max)*sizeof(BN_ULONG));
		p = b->d;
		if (p == NULL) return(NULL);

		memset(&(p[b->max]),0,((n+1)-b->max)*sizeof(BN_ULONG));
		b->max=n;
		wip_free(ret);
	}
		return(b);
//=====================================================================
}

BIGNUM *BN_Copy(a, b)
BIGNUM *a;
BIGNUM *b;
	{
	if (bn_expand(a,b->top*BN_BITS2) == NULL) return(NULL);
	memcpy(a->d,b->d,sizeof(b->d[0])*b->top);
	a->top=b->top;
	a->neg=b->neg;
	return(a);
	}

void BN_Clear(a)
BIGNUM *a;
	{
	memset(a->d,0,a->max*sizeof(a->d[0]));
	a->top=0;
	a->neg=0;
	}

int BN_SetWord(a,w)
BIGNUM *a;
unsigned long w;
	{
	int i,n;
	if (bn_expand(a,sizeof(unsigned long)*8) == NULL) return(0);

	n=sizeof(unsigned long)/BN_BYTES;
	a->neg=0;
	a->top=0;
	a->d[0]=(BN_ULONG)w&BN_MASK2;
	if (a->d[0] != 0) a->top=1;
	for (i=1; i<n; i++)
		{
		a->d[i]=(BN_ULONG)w&BN_MASK2;
		if (a->d[i] != 0) a->top=i+1;
		}
	return(1);
	}

BIGNUM *BN_Bin2Bn(s, len, ret)
unsigned char *s;
int len;
BIGNUM *ret;
{
	unsigned int i,m;
	unsigned int n;
	BN_ULONG l;

	if (ret == NULL) ret=BN_New();
	if (ret == NULL) return(NULL);
	l=0;
	n=len;
	if (n == 0) {
		ret->top=0;
		return(ret);
	}
	if (bn_expand(ret,(int)(n+2)*8) == NULL)
		return(NULL);
	i=((n-1)/BN_BYTES)+1;
	m=((n-1)%(BN_BYTES));
	ret->top=i;
	while (n-- > 0) {
		l=(l<<8L)| *(s++);
		if (m-- == 0) {
			ret->d[--i]=l;
			l=0;
			m=BN_BYTES-1;
		}
	}
	bn_fix_top(ret);
	return(ret);
}

int BN_Bn2Bin(a, to)
BIGNUM *a;
unsigned char *to;
	{
	int n,i;
	BN_ULONG l;

	n=i=BN_NumBytes(a);
	while (i-- > 0)
		{
		l=a->d[i/BN_BYTES];
		*(to++)=(unsigned char)(l>>(8*(i%BN_BYTES)))&0xff;
		}
	return(n);
	}

int BN_Ucmp(a, b)
BIGNUM *a;
BIGNUM *b;
	{
	int i;
	BN_ULONG t1,t2,*ap,*bp;

	i=a->top-b->top;
	if (i != 0) return(i);
	ap=a->d;
	bp=b->d;
	for (i=a->top-1; i>=0; i--)
		{
		t1= ap[i];
		t2= bp[i];
		if (t1 != t2)
			return(t1 > t2?1:-1);
		}
	return(0);
	}

int BN_Cmp(a, b)
BIGNUM *a;
BIGNUM *b;
	{
	int i;
	int gt,lt;
	BN_ULONG t1,t2;

	if ((a == NULL) || (b == NULL))
		{
		if (a != NULL)
			return(-1);
		else if (b != NULL)
			return(1);
		else
			return(0);
		}
	if (a->neg != b->neg)
		{
		if (a->neg)
			return(-1);
		else	return(1);
		}
	if (a->neg == 0)
		{ gt=1; lt= -1; }
	else	{ gt= -1; lt=1; }

	if (a->top > b->top) return(gt);
	if (a->top < b->top) return(lt);
	for (i=a->top-1; i>=0; i--)
		{
		t1=a->d[i];
		t2=b->d[i];
		if (t1 > t2) return(gt);
		if (t1 < t2) return(lt);
		}
	return(0);
	}

int BN_IsBitSet(a, n)
BIGNUM *a;
int n;
	{
	int i,j;

	if (n < 0) return(0);
	i=n/BN_BITS2;
	j=n%BN_BITS2;
	if (a->top <= i) return(0);
	return((a->d[i]&(((BN_ULONG)1)<<j))?1:0);
	}

int BN_MaskBits(a,n)
BIGNUM *a;
int n;
	{
	int b,w;

	w=n/BN_BITS2;
	b=n%BN_BITS2;
	if (w >= a->top) return(0);
	if (b == 0)
		a->top=w;
	else
		{
		a->top=w+1;
		a->d[w]&= ~(BN_MASK2<<b);
		while ((w >= 0) && (a->d[w] == 0))
			{
			a->top--;
			w--;
			}
		}
	return(1);
	}
