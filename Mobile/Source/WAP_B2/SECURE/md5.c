#include "md5_locl.h"

#define INIT_DATA_A (unsigned long)0x67452301L
#define INIT_DATA_B (unsigned long)0xefcdab89L
#define INIT_DATA_C (unsigned long)0x98badcfeL
#define INIT_DATA_D (unsigned long)0x10325476L

#ifndef NOPROTO
static void id_md5_block(MD5_CTX *c, unsigned long *p);
#else
static void id_md5_block();
#endif

//#define PURIFY
//#define L_ENDIAN

void ID_MD5_Init(c)
MD5_CTX *c;
	{
	c->A=INIT_DATA_A;
	c->B=INIT_DATA_B;
	c->C=INIT_DATA_C;
	c->D=INIT_DATA_D;
	c->Nl=0;
	c->Nh=0;
	c->num=0;
	}

void ID_MD5_Update(c, data, len)
MD5_CTX *c;
register unsigned char *data;
unsigned long len;
	{
	register ULONG *p;
	int sw,sc;
	ULONG l;

	if (len == 0) return;

	l=(c->Nl+(len<<3))&0xffffffffL;
	if (l < c->Nl)
		c->Nh++;
	c->Nh+=(len>>29);
	c->Nl=l;

	if (c->num != 0)
		{
		p=c->data;
		sw=c->num>>2;
		sc=c->num&0x03;

		if ((c->num+len) >= MD5_CBLOCK)
			{
			l= p[sw];
			p_c2l(data,l,sc);
			p[sw++]=l;
			for (; sw<MD5_LBLOCK; sw++)
				{
				c2l(data,l);
				p[sw]=l;
				}
			len-=(MD5_CBLOCK-c->num);

			id_md5_block(c,p);
			c->num=0;
			}
		else
			{
			int ew,ec;

			c->num+=(int)len;
			if ((sc+len) < 4)
				{
				l= p[sw];
				p_c2l_p(data,l,sc,len);
				p[sw]=l;
				}
			else
				{
				ew=(c->num>>2);
				ec=(c->num&0x03);
				l= p[sw];
				p_c2l(data,l,sc);
				p[sw++]=l;
				for (; sw < ew; sw++)
					{ c2l(data,l); p[sw]=l; }
				if (ec)
					{
					c2l_p(data,l,ec);
					p[sw]=l;
					}
				}
			return;
			}
		}
	p=c->data;
	while (len >= MD5_CBLOCK)
		{
#if defined(L_ENDIAN) || defined(B_ENDIAN)
		memcpy(p,data,MD5_CBLOCK);
		data+=MD5_CBLOCK;
#ifdef B_ENDIAN
		for (sw=(MD5_LBLOCK/4); sw; sw--)
			{
			Endian_Reverse32(p[0]);
			Endian_Reverse32(p[1]);
			Endian_Reverse32(p[2]);
			Endian_Reverse32(p[3]);
			p+=4;
			}
#endif
#else
		for (sw=(MD5_LBLOCK/4); sw; sw--)
			{
			c2l(data,l); *(p++)=l;
			c2l(data,l); *(p++)=l;
			c2l(data,l); *(p++)=l;
			c2l(data,l); *(p++)=l; 
			} 
#endif
		p=c->data;
		id_md5_block(c,p);
		len-=MD5_CBLOCK;
		}
	sc=(int)len;
	c->num=sc;
	if (sc)
		{
		sw=sc>>2;
#ifdef L_ENDIAN
		p[sw]=0;
		memcpy(p,data,sc);
#else
		sc&=0x03;
		for ( ; sw; sw--)
			{ c2l(data,l); *(p++)=l; }
		c2l_p(data,l,sc);
		*p=l;
#endif
		}
	}

static void id_md5_block(c, X)
MD5_CTX *c;
register ULONG *X;
	{
	register ULONG A,B,C,D;

	A=c->A;
	B=c->B;
	C=c->C;
	D=c->D;

	R0(A,B,C,D,X[ 0], 7,0xd76aa478L);
	R0(D,A,B,C,X[ 1],12,0xe8c7b756L);
	R0(C,D,A,B,X[ 2],17,0x242070dbL);
	R0(B,C,D,A,X[ 3],22,0xc1bdceeeL);
	R0(A,B,C,D,X[ 4], 7,0xf57c0fafL);
	R0(D,A,B,C,X[ 5],12,0x4787c62aL);
	R0(C,D,A,B,X[ 6],17,0xa8304613L);
	R0(B,C,D,A,X[ 7],22,0xfd469501L);
	R0(A,B,C,D,X[ 8], 7,0x698098d8L);
	R0(D,A,B,C,X[ 9],12,0x8b44f7afL);
	R0(C,D,A,B,X[10],17,0xffff5bb1L);
	R0(B,C,D,A,X[11],22,0x895cd7beL);
	R0(A,B,C,D,X[12], 7,0x6b901122L);
	R0(D,A,B,C,X[13],12,0xfd987193L);
	R0(C,D,A,B,X[14],17,0xa679438eL);
	R0(B,C,D,A,X[15],22,0x49b40821L);

	R1(A,B,C,D,X[ 1], 5,0xf61e2562L);
	R1(D,A,B,C,X[ 6], 9,0xc040b340L);
	R1(C,D,A,B,X[11],14,0x265e5a51L);
	R1(B,C,D,A,X[ 0],20,0xe9b6c7aaL);
	R1(A,B,C,D,X[ 5], 5,0xd62f105dL);
	R1(D,A,B,C,X[10], 9,0x02441453L);
	R1(C,D,A,B,X[15],14,0xd8a1e681L);
	R1(B,C,D,A,X[ 4],20,0xe7d3fbc8L);
	R1(A,B,C,D,X[ 9], 5,0x21e1cde6L);
	R1(D,A,B,C,X[14], 9,0xc33707d6L);
	R1(C,D,A,B,X[ 3],14,0xf4d50d87L);
	R1(B,C,D,A,X[ 8],20,0x455a14edL);
	R1(A,B,C,D,X[13], 5,0xa9e3e905L);
	R1(D,A,B,C,X[ 2], 9,0xfcefa3f8L);
	R1(C,D,A,B,X[ 7],14,0x676f02d9L);
	R1(B,C,D,A,X[12],20,0x8d2a4c8aL);

	R2(A,B,C,D,X[ 5], 4,0xfffa3942L);
	R2(D,A,B,C,X[ 8],11,0x8771f681L);
	R2(C,D,A,B,X[11],16,0x6d9d6122L);
	R2(B,C,D,A,X[14],23,0xfde5380cL);
	R2(A,B,C,D,X[ 1], 4,0xa4beea44L);
	R2(D,A,B,C,X[ 4],11,0x4bdecfa9L);
	R2(C,D,A,B,X[ 7],16,0xf6bb4b60L);
	R2(B,C,D,A,X[10],23,0xbebfbc70L);
	R2(A,B,C,D,X[13], 4,0x289b7ec6L);
	R2(D,A,B,C,X[ 0],11,0xeaa127faL);
	R2(C,D,A,B,X[ 3],16,0xd4ef3085L);
	R2(B,C,D,A,X[ 6],23,0x04881d05L);
	R2(A,B,C,D,X[ 9], 4,0xd9d4d039L);
	R2(D,A,B,C,X[12],11,0xe6db99e5L);
	R2(C,D,A,B,X[15],16,0x1fa27cf8L);
	R2(B,C,D,A,X[ 2],23,0xc4ac5665L);

	R3(A,B,C,D,X[ 0], 6,0xf4292244L);
	R3(D,A,B,C,X[ 7],10,0x432aff97L);
	R3(C,D,A,B,X[14],15,0xab9423a7L);
	R3(B,C,D,A,X[ 5],21,0xfc93a039L);
	R3(A,B,C,D,X[12], 6,0x655b59c3L);
	R3(D,A,B,C,X[ 3],10,0x8f0ccc92L);
	R3(C,D,A,B,X[10],15,0xffeff47dL);
	R3(B,C,D,A,X[ 1],21,0x85845dd1L);
	R3(A,B,C,D,X[ 8], 6,0x6fa87e4fL);
	R3(D,A,B,C,X[15],10,0xfe2ce6e0L);
	R3(C,D,A,B,X[ 6],15,0xa3014314L);
	R3(B,C,D,A,X[13],21,0x4e0811a1L);
	R3(A,B,C,D,X[ 4], 6,0xf7537e82L);
	R3(D,A,B,C,X[11],10,0xbd3af235L);
	R3(C,D,A,B,X[ 2],15,0x2ad7d2bbL);
	R3(B,C,D,A,X[ 9],21,0xeb86d391L);

	c->A+=A&0xffffffffL;
	c->B+=B&0xffffffffL;
	c->C+=C&0xffffffffL;
	c->D+=D&0xffffffffL;
	}

void ID_MD5_Final(md, c)
unsigned char *md;
MD5_CTX *c;
	{
	register int i,j;
	register ULONG l;
	register ULONG *p;
	static unsigned char end[4]={0x80,0x00,0x00,0x00};
	unsigned char *cp=end;

	p=c->data;
	j=c->num;
	i=j>>2;

#ifdef PURIFY
	if ((j&0x03) == 0) p[i]=0;
#endif
	l=p[i];
	p_c2l(cp,l,j&0x03);
	p[i]=l;
	i++;
	if (c->num >= MD5_LAST_BLOCK)
		{
		for (; i<MD5_LBLOCK; i++)
			p[i]=0;
		id_md5_block(c,p);
		i=0;
		}
	for (; i<(MD5_LBLOCK-2); i++)
		p[i]=0;
	p[MD5_LBLOCK-2]=c->Nl;
	p[MD5_LBLOCK-1]=c->Nh;
	id_md5_block(c,p);
	cp=md;
	l=c->A; l2c(l,cp);
	l=c->B; l2c(l,cp);
	l=c->C; l2c(l,cp);
	l=c->D; l2c(l,cp);

	c->num=0;
	}

unsigned char *ID_MD5_Block(d, n, md)
unsigned char *d;
unsigned long n;
unsigned char *md;
	{
	MD5_CTX c;
	static unsigned char m[MD5_DIGEST_LENGTH];

	if (md == NULL) md=m;
	ID_MD5_Init(&c);
	ID_MD5_Update(&c,d,n);
	ID_MD5_Final(md,&c);
	memset(&c,0,sizeof(c));
	return(md);
	}

