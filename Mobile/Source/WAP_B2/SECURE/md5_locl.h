#include <stdlib.h>
#include <string.h>
#include "md5.h"

#define ULONG	unsigned long
#define UCHAR	unsigned char
#define UINT	unsigned int

#if defined(NOCONST)
#define const
#endif

#undef c2l
#define c2l(c,l)	(l = ((unsigned long)(*((c)++)))     , \
			 l|=(((unsigned long)(*((c)++)))<< 8), \
			 l|=(((unsigned long)(*((c)++)))<<16), \
			 l|=(((unsigned long)(*((c)++)))<<24))

#undef p_c2l
#define p_c2l(c,l,n)	{ \
			switch (n) { \
			case 0: l =((unsigned long)(*((c)++))); \
			case 1: l|=((unsigned long)(*((c)++)))<< 8; \
			case 2: l|=((unsigned long)(*((c)++)))<<16; \
			case 3: l|=((unsigned long)(*((c)++)))<<24; \
				} \
			}

#undef c2l_p
#define c2l_p(c,l,n)	{ \
			l=0; \
			(c)+=n; \
			switch (n) { \
			case 3: l =((unsigned long)(*(--(c))))<<16; \
			case 2: l|=((unsigned long)(*(--(c))))<< 8; \
			case 1: l|=((unsigned long)(*(--(c))))    ; \
				} \
			}

#undef p_c2l_p
#define p_c2l_p(c,l,sc,len) { \
			switch (sc) \
				{ \
			case 0: l =((unsigned long)(*((c)++))); \
				if (--len == 0) break; \
			case 1: l|=((unsigned long)(*((c)++)))<< 8; \
				if (--len == 0) break; \
			case 2: l|=((unsigned long)(*((c)++)))<<16; \
				} \
			}

#undef l2c
#define l2c(l,c)	(*((c)++)=(unsigned char)(((l)    )&0xff), \
			 *((c)++)=(unsigned char)(((l)>> 8)&0xff), \
			 *((c)++)=(unsigned char)(((l)>>16)&0xff), \
			 *((c)++)=(unsigned char)(((l)>>24)&0xff))

#undef l2cn
#define l2cn(l1,l2,c,n)	{ \
			c+=n; \
			switch (n) { \
			case 8: *(--(c))=(unsigned char)(((l2)>>24)&0xff); \
			case 7: *(--(c))=(unsigned char)(((l2)>>16)&0xff); \
			case 6: *(--(c))=(unsigned char)(((l2)>> 8)&0xff); \
			case 5: *(--(c))=(unsigned char)(((l2)    )&0xff); \
			case 4: *(--(c))=(unsigned char)(((l1)>>24)&0xff); \
			case 3: *(--(c))=(unsigned char)(((l1)>>16)&0xff); \
			case 2: *(--(c))=(unsigned char)(((l1)>> 8)&0xff); \
			case 1: *(--(c))=(unsigned char)(((l1)    )&0xff); \
				} \
			}

#if defined(WIN32)
#define Endian_Reverse32(a) \
	{ \
	unsigned long l=(a); \
	(a)=((ROTATE(l,8)&0x00FF00FF)|(ROTATE(l,24)&0xFF00FF00)); \
	}
#else
#define Endian_Reverse32(a) \
	{ \
	unsigned long l=(a); \
	l=(((l&0xFF00FF00)>>8L)|((l&0x00FF00FF)<<8L)); \
	(a)=ROTATE(l,16L); \
	}
#endif

#define	F(x,y,z)	((((y) ^ (z)) & (x)) ^ (z))
#define	G(x,y,z)	((((x) ^ (y)) & (z)) ^ (y))
#define	H(x,y,z)	((x) ^ (y) ^ (z))
#define	I(x,y,z)	(((x) | (~(z))) ^ (y))

#undef ROTATE
#if defined(WIN32)
#define ROTATE(a,n)     _lrotl(a,n)
#else
#define ROTATE(a,n)     (((a)<<(n))|(((a)&0xffffffff)>>(32-(n))))
#endif


#define R0(a,b,c,d,k,s,t) { \
	a+=((k)+(t)+F((b),(c),(d))); \
	a=ROTATE(a,s); \
	a+=b; };\

#define R1(a,b,c,d,k,s,t) { \
	a+=((k)+(t)+G((b),(c),(d))); \
	a=ROTATE(a,s); \
	a+=b; };

#define R2(a,b,c,d,k,s,t) { \
	a+=((k)+(t)+H((b),(c),(d))); \
	a=ROTATE(a,s); \
	a+=b; };

#define R3(a,b,c,d,k,s,t) { \
	a+=((k)+(t)+I((b),(c),(d))); \
	a=ROTATE(a,s); \
	a+=b; };
