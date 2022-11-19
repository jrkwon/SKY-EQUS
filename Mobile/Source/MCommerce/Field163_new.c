#include "Field163.h"
#include "Macro.h"

/* 
	Field : GF(2^163)

	Irreducible Polynomial is a Pentanomial (163 8 2 1)

	Made by Suengchur Pyun!! (2000/09/10)

	Updated by Seungchur Pyun!! (2001/06/05)
	(Content : Curve 3 ==> Curve 5)

	e-mail : pyun@postech.edu
*/

static WORD16	sqrtab[256];
static int is_Init_Sq = 0;

int IsEqual163(WORD32 *A, WORD32 *B)
{
	int i;
	for(i=0; i<6; i++) if(A[i] != B[i]) return 0;
	return 1;
}

int Copy163(WORD32 *B, WORD32 *A)
{
	int i;
	for(i=0; i<6; i++) B[i] = A[i];
	return 1;
}

int Add163(WORD32 *C, WORD32 *A, WORD32 *B)
{
	int i;
	for(i=0; i<6; i++) C[i] = A[i]^B[i];
	return 1;
}

int Print163(const char *str, WORD32 *A)
{
	int i;
	printf("%s", str);
	printf("%.1lx ", A[5]&0x7);
	for(i=4; i>=0; i--) printf("%.8lx ", A[i]);
	printf("\n");
	return 1;
}

int Field2Bn(BIGNUM *B, WORD32 *A)
{
	int i;
	B->top = 0;
	bn_wexpand(B, 6);
	for(i=0; i<6; i++)
	{
		if(A[i] != 0)
		{
			B->top = i+1;
		}
		B->d[i] = A[i];
	}

	return 1;
}

int Field2Octet(unsigned char *to, WORD32 *A, int fsize)
{
	int n, i;
	WORD32 l;

	n=i=(fsize/8 + ((fsize%8==0) ? 0 : 1));
	while (i-- > 0)
	{
		l=A[i/4];
		*(to++)=(unsigned char)(l>>(8*(i%4)))&0xff;
	}
	return(n);
}

int Octet2Field(WORD32 *A, unsigned char *B, int octlen)
{
	BIGNUM *Big=NULL;
	int size;
	if((Big=BN_new())==NULL) return 0;
	BN_bin2bn(B, octlen, Big);
	size = octlen/4 + ((octlen%4==0) ? 0 : 1);

	Bn2Field(A, Big, size);
	BN_clear_free(Big);
	return size;
}

int Bn2Field(WORD32 *B, BIGNUM *A, int size)
{
	int i;
	for(i=0; i<size; i++) B[i] = 0;
	for(i=0; i<A->top; i++) B[i] = A->d[i];
	return 1;
}



int Init_SqrTab()
{
	short i;
	for(i=0; i<256; i++)
		sqrtab[i] = ((i&0x80)<<7) + ((i&0x40)<<6) + ((i&0x20)<<5) +\
		((i&0x10)<<4) + ((i&0x8)<<3) + ((i&0x4)<<2) + ((i&0x2)<<1) + (i&1);
	is_Init_Sq = 1;
	return 1;
}

int Sqr163(WORD32 *C, WORD32 *A)
{
	WORD32 a0, a1, a2, a3, a4, a5;
	WORD32 a6, a7, a8, a9, a10;
	WORD32 temp;

	if(!is_Init_Sq) Init_SqrTab();
	temp = A[0];
	a0 = sqrtab[temp&0xff]^(sqrtab[(temp>>8)&0xff]<<16);
	a1 = sqrtab[(temp>>16)&0xff]^(sqrtab[temp>>24]<<16);

	temp = A[1];
	a2 = sqrtab[temp&0xff]^(sqrtab[(temp>>8)&0xff]<<16);
	a3 = sqrtab[(temp>>16)&0xff]^(sqrtab[temp>>24]<<16);

	temp = A[2];
	a4 = sqrtab[temp&0xff]^(sqrtab[(temp>>8)&0xff]<<16);
	a5 = sqrtab[(temp>>16)&0xff]^(sqrtab[temp>>24]<<16);

	temp = A[3];
	a6 = sqrtab[temp&0xff]^(sqrtab[(temp>>8)&0xff]<<16);
	a7 = sqrtab[(temp>>16)&0xff]^(sqrtab[temp>>24]<<16);

	temp = A[4];
	a8 = sqrtab[temp&0xff]^(sqrtab[(temp>>8)&0xff]<<16);
	a9 = sqrtab[(temp>>16)&0xff]^(sqrtab[temp>>24]<<16);

	temp = A[5];
	a10 = sqrtab[temp&0x7];

	a5 ^= ((((a10&0x1f)<<5)|(a9>>27))^((a10>>1)&0xf)^((a10>>2)&0x7)^((a10>>3)&0x3));
	a4 	^= (((a9<<5)|(a8>>27))^((a10<<31)|(a9>>1))^((a10<<30)|(a9>>2))^((a10<<29)|(a9>>3)));
	a3 	^= (((a8<<5)|(a7>>27))^((a9<<31)|(a8>>1))^((a9<<30)|(a8>>2))^((a9<<29)|(a8>>3)));
	a2 ^= (((a7<<5)|(a6>>27))^((a8<<31)|(a7>>1))^((a8<<30)|(a7>>2))^((a8<<29)|(a7>>3)));
	a1 ^= (((a6<<5)|(a5>>27))^((a7<<31)|(a6>>1))^((a7<<30)|(a6>>2))^((a7<<29)|(a6>>3)));
	a0 ^= (((a5<<5)&~0xff)^(((a6<<31)|(a5>>1))&~0x3)^(((a6<<30)|(a5>>2))&~0x1)^((a6<<29)|(a5>>3)));

	C[5] = a5&0x00000007;
	C[4] = a4;
	C[3] = a3;
	C[2] = a2;
	C[1] = a1;
	C[0] = a0;
	
	return 1;
}

#define MACRO_MUL163_1BIT(n, a, b, c) if((b)&(MASK##n))	{							\
	c##0 ^= a##0<<n; c##1 ^= a##0>>BIT32_##n;												\
	c##1 ^= a##1<<n; c##2 ^= a##1>>BIT32_##n;												\
	c##2 ^= a##2<<n; c##3 ^= a##2>>BIT32_##n;												\
	c##3 ^= a##3<<n; c##4 ^= a##3>>BIT32_##n;												\
	c##4 ^= a##4<<n; c##5 ^= a##4>>BIT32_##n;												\
	c##5 ^= a##5<<n; c##6 ^= a##5>>BIT32_##n; }											\

static void MUL163_ONE_WORD32(WORD32 *c, WORD32 A0, WORD32 A1, \
	WORD32 A2, WORD32 A3, WORD32 A4, WORD32 A5, WORD32 B)								
{		
	WORD32 a0=A0, a1=A1, a2=A2, a3=A3, a4=A4, a5=A5;
	WORD32 b=B;
	WORD32 c0, c1, c2, c3, c4, c5, c6;

	if(b & MASK31)																		
	{																						
		c0 = a0<<31;																
		c1 = (a0>>1) | (a1<<31);											
		c2 = (a1>>1) | (a2<<31);									
		c3 = (a2>>1) | (a3<<31);									
		c4 = (a3>>1) | (a4<<31);									
		c5 = (a4>>1) | (a5<<31);									
		c6 = (a5>>1);														
	}else																					
	{																						
		c0 = 0; c1 = 0; c2 = 0; c3 = 0;								
		c4 = 0; c5 = 0; c6 = 0;															
	}																						
	MACRO_MUL163_1BIT(30, a, b, c); MACRO_MUL163_1BIT(29, a, b, c);	
	MACRO_MUL163_1BIT(28, a, b, c); MACRO_MUL163_1BIT(27, a, b, c);	
	MACRO_MUL163_1BIT(26, a, b, c); MACRO_MUL163_1BIT(25, a, b, c);	
	MACRO_MUL163_1BIT(24, a, b, c); MACRO_MUL163_1BIT(23, a, b, c);	
	MACRO_MUL163_1BIT(22, a, b, c); MACRO_MUL163_1BIT(21, a, b, c);	
	MACRO_MUL163_1BIT(20, a, b, c); MACRO_MUL163_1BIT(19, a, b, c);	
	MACRO_MUL163_1BIT(18, a, b, c); MACRO_MUL163_1BIT(17, a, b, c);	
	MACRO_MUL163_1BIT(16, a, b, c); MACRO_MUL163_1BIT(15, a, b, c);	
	MACRO_MUL163_1BIT(14, a, b, c); MACRO_MUL163_1BIT(13, a, b, c);	
	MACRO_MUL163_1BIT(12, a, b, c); MACRO_MUL163_1BIT(11, a, b, c);	
	MACRO_MUL163_1BIT(10, a, b, c); MACRO_MUL163_1BIT(9, a, b, c);		
	MACRO_MUL163_1BIT(8, a, b, c);  MACRO_MUL163_1BIT(7, a, b, c);		
	MACRO_MUL163_1BIT(6, a, b, c);  MACRO_MUL163_1BIT(5, a, b, c);		
	MACRO_MUL163_1BIT(4, a, b, c);  MACRO_MUL163_1BIT(3, a, b, c);		
	MACRO_MUL163_1BIT(2, a, b, c);  MACRO_MUL163_1BIT(1, a, b, c);		
	if((b) & MASK0)																	
	{																						
		c0 ^= a0; c1 ^= a1; c2 ^= a2;				
		c3 ^= a3; c4 ^= a4; c5 ^= a5;				
	}
	c[0] = c0; c[1] = c1; c[2] = c2; c[3] = c3; c[4] = c4; 
	c[5] = c5; c[6] = c6;
}

static void MUL163_ONE_WORD3(WORD32 *c, WORD32 A0, WORD32 A1, WORD32 A2, \
							 WORD32 A3, WORD32 A4, WORD32 A5, WORD32 B)						
{
	WORD32 a0=A0, a1=A1, a2=A2, a3=A3, a4=A4, a5=A5;
	WORD32 b = B;
	WORD32 c0=0, c1=0, c2=0, c3=0, c4=0, c5=0, c6=0;

   MACRO_MUL163_1BIT(2, a, b, c); MACRO_MUL163_1BIT(1, a, b, c);		
	if((b) & MASK0)																	
	{																						
		c0 ^= a0; c1 ^= a1; c2 ^= a2;				
		c3 ^= a3; c4 ^= a4; c5 ^= a5;				
	}						
	c[0] = c0; c[1] = c1; c[2] = c2; c[3] = c3; c[4] = c4; 
	c[5] = c5; c[6] = c6;	
}

int Mul163(WORD32 *C, WORD32 *A, WORD32 *B)
{
	WORD32 temp[7];
	WORD32 buffer0, buffer1, buffer2, buffer3, buffer4, buffer5;
	WORD32 buffer6, buffer7, buffer8, buffer9, buffer10;

	MUL163_ONE_WORD32(temp, A[0], A[1], A[2], A[3], A[4], A[5], B[0]);
	buffer0 = temp[0]; buffer1 = temp[1];
	buffer2 = temp[2]; buffer3 = temp[3];
	buffer4 = temp[4]; buffer5 = temp[5];
	buffer6 = temp[6]; 

	MUL163_ONE_WORD32(temp, A[0], A[1], A[2], A[3], A[4], A[5], B[1]);
	buffer1 ^= temp[0]; buffer2 ^= temp[1];
	buffer3 ^= temp[2]; buffer4 ^= temp[3];
	buffer5 ^= temp[4]; buffer6 ^= temp[5];
	buffer7  = temp[6];

	MUL163_ONE_WORD32(temp, A[0], A[1], A[2], A[3], A[4], A[5], B[2]);
	buffer2 ^= temp[0]; buffer3 ^= temp[1];
	buffer4 ^= temp[2]; buffer5 ^= temp[3];
	buffer6 ^= temp[4]; buffer7 ^= temp[5];
	buffer8  = temp[6];

	MUL163_ONE_WORD32(temp, A[0], A[1], A[2], A[3], A[4], A[5], B[3]);
	buffer3 ^= temp[0]; buffer4 ^= temp[1];
	buffer5 ^= temp[2]; buffer6 ^= temp[3];
	buffer7 ^= temp[4]; buffer8 ^= temp[5];
	buffer9  = temp[6];

	MUL163_ONE_WORD32(temp, A[0], A[1], A[2], A[3], A[4], A[5], B[4]);
	buffer4 ^= temp[0]; buffer5 ^= temp[1];
	buffer6 ^= temp[2]; buffer7 ^= temp[3];
	buffer8 ^= temp[4]; buffer9 ^= temp[5];
	buffer10 = temp[6];

	MUL163_ONE_WORD3(temp, A[0], A[1], A[2], A[3], A[4], A[5], B[5]);
	buffer5 ^= temp[0]; buffer6 ^= temp[1];
	buffer7 ^= temp[2]; buffer8 ^= temp[3];
	buffer9 ^= temp[4]; buffer10 ^= temp[5];

	buffer5 ^= ((((buffer10&0x1f)<<5)|(buffer9>>27))^((buffer10>>1)&0xf)^((buffer10>>2)&0x7)^((buffer10>>3)&0x3));
	buffer4 ^= (((buffer9<<5)|(buffer8>>27))^((buffer10<<31)|(buffer9>>1))^((buffer10<<30)|(buffer9>>2))^((buffer10<<29)|(buffer9>>3)));
	buffer3 ^= (((buffer8<<5)|(buffer7>>27))^((buffer9<<31)|(buffer8>>1))^((buffer9<<30)|(buffer8>>2))^((buffer9<<29)|(buffer8>>3)));
	buffer2 ^= (((buffer7<<5)|(buffer6>>27))^((buffer8<<31)|(buffer7>>1))^((buffer8<<30)|(buffer7>>2))^((buffer8<<29)|(buffer7>>3)));
	buffer1 ^= (((buffer6<<5)|(buffer5>>27))^((buffer7<<31)|(buffer6>>1))^((buffer7<<30)|(buffer6>>2))^((buffer7<<29)|(buffer6>>3)));
	buffer0 ^= (((buffer5<<5)&~0xff)^(((buffer6<<31)|(buffer5>>1))&~0x3)^(((buffer6<<30)|(buffer5>>2))&~0x1)^((buffer6<<29)|(buffer5>>3)));

	C[5] = buffer5&0x00000007;
	C[4] = buffer4;
	C[3] = buffer3;
	C[2] = buffer2;
	C[1] = buffer1;
	C[0] = buffer0;

	return 1;
}

int IsZero163(WORD32 *A)
{
	if(A[0] == 0 && A[1] == 0 && A[2] == 0 &&\
		A[3] == 0 && A[4] == 0 && A[5] == 0) return 1;
	return 0;
}

int SetZero163(WORD32 *A)
{
	A[0] = 0; A[1] = 0; A[2] = 0; A[3] = 0;
	A[4] = 0; A[5] = 0;
	return 1;
}


static int Divide163(WORD32 *C, WORD32 k)
{
	WORD32 a0, a1, a2, a3, a4, a5;
	WORD32 tmp, m;
	
	a0 = C[0]; a1 = C[1]; a2 = C[2];
	a3 = C[3]; a4 = C[4]; a5 = C[5];

	for(; k >= 16; k -= 16)
	{
		tmp = (a0&0x0000ffff);
		a0 = ((a1<<16)|(a0>>16))^(tmp)^(tmp<<4)^(tmp<<5)^(tmp<<6)^(tmp<<7);
		a1 = ((a2<<16)|(a1>>16));
		a2 = ((a3<<16)|(a2>>16));
		a3 = ((a4<<16)|(a3>>16));
		a4 = ((a5<<16)|(a4>>16))^(tmp<<19)^(tmp<<20)^(tmp<<22)
		^(tmp<<23)^(tmp<<25)^(tmp<<26)^(tmp<<27)
		^(tmp<<28)^(tmp<<31);
		a5 = (a5>>16)^(tmp>>13)^(tmp>>12)^(tmp>>10)^(tmp>>9)
		^(tmp>>7)^(tmp>>6)^(tmp>>5)^(tmp>>4)^(tmp>>1)
		^tmp^(tmp<<1)^(tmp<<2);
	}

	for(; k >= 6; k -= 6)
	{
		tmp = (a0&0x0000003f);
		a0 = ((a1<<26)|(a0>>6))^(tmp)^(tmp<<2)^(tmp<<3)^(tmp<<5)^(tmp<<6);
		a1 = ((a2<<26)|(a1>>6));
		a2 = ((a3<<26)|(a2>>6));
		a3 = ((a4<<26)|(a3>>6));
		a4 = ((a5<<26)|(a4>>6))^(tmp<<29)^(tmp<<30);
		a5 = (a5>>6)^(tmp>>3)^(tmp>>2)^(tmp)^(tmp<<1);
	}

	if(k)
	{
		m = 32-k;
		tmp = (a0<<m)>>m;
		a0 = ((a1<<m)|(a0>>k))^(tmp<<(6-k))^(tmp<<(8-k))^(tmp<<(9-k))
		^(tmp<<(11-k))^(tmp<<(12-k));
		a1 = ((a2<<m)|(a1>>k));
		a2 = ((a3<<m)|(a2>>k));
		a3 = ((a4<<m)|(a3>>k));
		a4 = ((a5<<m)|(a4>>k));

		if(35-k<32) a4 ^= (tmp<<(35-k));
		if(36-k<32) a4 ^= (tmp<<(36-k));

		a5 = (a5>>k)^(tmp<<(6-k))^(tmp<<(7-k));
		if(k>=3) a5 ^= (tmp>>(k-3));
		else a5 ^= (tmp<<(3-k));

		if(k>=4) a5 ^= (tmp>>(k-4));
		else a5 ^= (tmp<<(4-k));
	}

	tmp = a5>>3;
	C[5] = a5 & 0x00000007;
	C[4] = a4;
	C[3] = a3;
	C[2] = a2;
	C[1] = a1;
	C[0] = a0^tmp^(tmp<<1)^(tmp<<2)^(tmp<<8);

	return 1;
}

#define ADDMACVAR6(a,b) b##0^=a##0; b##1^=a##1; b##2^=a##2; b##3^=a##3; \
b##4^=a##4; b##5^=a##5;
#define ADDMACVAR5(a,b) b##0^=a##0; b##1^=a##1; b##2^=a##2; b##3^=a##3; b##4^=a##4;
#define ADDMACVAR4(a,b) b##0^=a##0; b##1^=a##1; b##2^=a##2; b##3^=a##3;
#define ADDMACVAR3(a,b) b##0^=a##0; b##1^=a##1; b##2^=a##2;
#define ADDMACVAR2(a,b) b##0^=a##0; b##1^=a##1;
#define ADDMACVAR1(a,b) b##0^=a##0;


#define LSHMACVAR6(a)  a##5<<=1; a##5^=a##4>>31;\
  a##4<<=1; a##4^=a##3>>31; a##3<<=1; a##3^=a##2>>31;  \
  a##2<<=1; a##2^=a##1>>31; a##1<<=1; a##1^=a##0>>31; a##0<<=1;
#define LSHMACVAR5(a)  \
  a##4<<=1; a##4^=a##3>>31; a##3<<=1; a##3^=a##2>>31;  \
  a##2<<=1; a##2^=a##1>>31; a##1<<=1; a##1^=a##0>>31; a##0<<=1;
#define LSHMACVAR4(a)  \
  a##3<<=1; a##3^=a##2>>31;  \
  a##2<<=1; a##2^=a##1>>31; a##1<<=1; a##1^=a##0>>31; a##0<<=1;
#define LSHMACVAR3(a)  \
  a##2<<=1; a##2^=a##1>>31; a##1<<=1; a##1^=a##0>>31; a##0<<=1;
#define LSHMACVAR2(a)  a##1<<=1; a##1^=a##0>>31; a##0<<=1;
#define LSHMACVAR1(a)  a##0<<=1;


#define RSHMACVAR6(a) \
  a##0>>=1; a##0^=a##1<<31; a##1>>=1; a##1^=a##2<<31;  \
  a##2>>=1; a##2^=a##3<<31; a##3>>=1; a##3^=a##4<<31;  \
  a##4>>=1; a##4^=a##5<<31; a##5>>=1;
#define RSHMACVAR5(a) \
  a##0>>=1; a##0^=a##1<<31; a##1>>=1; a##1^=a##2<<31;  \
  a##2>>=1; a##2^=a##3<<31; a##3>>=1; a##3^=a##4<<31; a##4>>=1; 
#define RSHMACVAR4(a) \
  a##0>>=1; a##0^=a##1<<31; a##1>>=1; a##1^=a##2<<31;  \
  a##2>>=1; a##2^=a##3<<31; a##3>>=1;
#define RSHMACVAR3(a) \
  a##0>>=1; a##0^=a##1<<31; a##1>>=1; a##1^=a##2<<31; a##2>>=1;
#define RSHMACVAR2(a) a##0>>=1; a##0^=a##1<<31; a##1>>=1;
#define RSHMACVAR1(a) a##0>>=1;


#define IF1MAC6(a)  if ((a##0==1) && !a##1 && !a##2 && !a##3 && !a##4 && !a##5)
#define IF1MAC5(a)  if ((a##0==1) && !a##1 && !a##2 && !a##3 && !a##4)
#define IF1MAC4(a)  if ((a##0==1) && !a##1 && !a##2 && !a##3)
#define IF1MAC3(a)  if ((a##0==1) && !a##1 && !a##2)
#define IF1MAC2(a)  if ((a##0==1) && !a##1)
#define IF1MAC1(a)  if (a##0==1)


#define COPYMAC(a,b) \
  b[0]=a##0; b[1]=a##1; b[2]=a##2; b[3]=a##3; b[4]=a##4; b[5]=a##5;


#define COMPAREMAC6(a,b,tag,tagout) \
  if (a##5>b##5) goto tag; if (a##5<b##5) goto tag##2; if (!a##5) goto tagout; \
  if (a##4>b##4) goto tag; if (a##4<b##4) goto tag##2; \
  if (a##3>b##3) goto tag; if (a##3<b##3) goto tag##2; \
  if (a##2>b##2) goto tag; if (a##2<b##2) goto tag##2; \
  if (a##1>b##1) goto tag; if (a##1<b##1) goto tag##2; \
  if (a##0>b##0) goto tag;
#define COMPAREMAC5(a,b,tag,tagout) \
  if (a##4>b##4) goto tag; if (a##4<b##4) goto tag##2; if (!a##4) goto tagout; \
  if (a##3>b##3) goto tag; if (a##3<b##3) goto tag##2; \
  if (a##2>b##2) goto tag; if (a##2<b##2) goto tag##2; \
  if (a##1>b##1) goto tag; if (a##1<b##1) goto tag##2; \
  if (a##0>b##0) goto tag;
#define COMPAREMAC4(a,b,tag,tagout) \
  if (a##3>b##3) goto tag; if (a##3<b##3) goto tag##2; if (!a##3) goto tagout; \
  if (a##2>b##2) goto tag; if (a##2<b##2) goto tag##2; \
  if (a##1>b##1) goto tag; if (a##1<b##1) goto tag##2; \
  if (a##0>b##0) goto tag;
#define COMPAREMAC3(a,b,tag,tagout) \
  if (a##2>b##2) goto tag; if (a##2<b##2) goto tag##2; if (!a##2) goto tagout; \
  if (a##1>b##1) goto tag; if (a##1<b##1) goto tag##2; \
  if (a##0>b##0) goto tag;
#define COMPAREMAC2(a,b,tag,tagout) \
  if (a##1>b##1) goto tag; if (a##1<b##1) goto tag##2; if (!a##1) goto tagout; \
  if (a##0>b##0) goto tag;
#define COMPAREMAC1(a,b,tag,tagout) \
  if (a##0>b##0) goto tag;


#define KEEP5(x) 
#define KEEP4(x) x
#define KEEP3(x) x
#define KEEP2(x) x
#define KEEP1(x) x
#define KEEP0(x) x


#define INVHMACINNER(M,MP1,MM1,N,NM1) \
 tag##M##N##add:  ADDMACVAR##N(mod,a); ADDMACVAR##M(modc,ac);   \
 tag##M##N##sh:   shcnt++; RSHMACVAR##N(a); LSHMACVAR##M(modc); \
  KEEP##MM1( if ((long)modc##MM1<0) goto tag##MP1##N; ) \
 tag##M##N: \
  if ((a0 & 1) == 0) goto tag##M##N##sh;  \
  IF1MAC##N(a) { goto axit; }  \
 tag##M##N##cmp:  \
  COMPAREMAC##N(a,mod,tag##M##N##add,tag##M##NM1##cmp);  \
  /* swap names, fall into add */  \
 tag##M##N##add2:  ADDMACVAR##N(a,mod); ADDMACVAR##M(ac,modc);  \
 tag##M##N##sh2:   shcnt++; RSHMACVAR##N(mod); LSHMACVAR##M(ac);\
  KEEP##MM1( if ((long)ac##MM1<0) goto tag##MP1##N##2; ) \
 tag##M##N##2:  \
  if ((mod0 & 1) == 0) goto tag##M##N##sh2; \
  IF1MAC##N(mod) { goto modxit; }  \
  goto tag##M##N##cmp;


#define INVHMACOUTER(K,KP1,KM1) INVHMACINNER(K,KP1,KM1,6,5)      \
        INVHMACINNER(K,KP1,KM1,5,4) INVHMACINNER(K,KP1,KM1,4,3)  \
        INVHMACINNER(K,KP1,KM1,3,2) INVHMACINNER(K,KP1,KM1,2,1) INVHMACINNER(K,KP1,KM1,1,0)

static WORD32 AlmostInv163(WORD32 *C, WORD32 *A, WORD32 *Penta163)
{
	WORD32 a0, a1, a2, a3, a4, a5;
	WORD32 mod0, mod1, mod2, mod3, mod4, mod5;
	WORD32 ac0, ac1, ac2, ac3, ac4, ac5;
	WORD32 modc0, modc1, modc2, modc3, modc4, modc5;
	WORD32 shcnt=0;

	a0 = A[0]; a1 = A[1]; a2 = A[2]; 
	a3 = A[3]; a4 = A[4]; a5 = A[5];
	mod0 = Penta163[0]; mod1 = Penta163[1]; 
	mod2 = Penta163[2]; mod3 = Penta163[3];
	mod4 = Penta163[4]; mod5 = Penta163[5];

	ac0=1; ac1=ac2=ac3=ac4=ac5=0;
	modc0=modc1=modc2=modc3=modc4=modc5=0;

	goto tag16;

	INVHMACOUTER(1, 2, 0)
	INVHMACOUTER(2, 3, 1)
	INVHMACOUTER(3, 4, 2)
	INVHMACOUTER(4, 5, 3)
	INVHMACOUTER(5, 6, 4)
	INVHMACOUTER(6, 7, 5)

axit:
	COPYMAC(ac, C); return shcnt;
modxit:
	COPYMAC(modc, C); return shcnt;

	goto tag61add2; goto tag62add2; goto tag63add2; goto tag64add2; 
	goto tag65add2; goto tag66add2;

	goto tag51add2; goto tag52add2; goto tag53add2; goto tag54add2; 
	goto tag55add2; goto tag56add2;

	goto tag41add2; goto tag42add2; goto tag43add2; goto tag44add2; 
	goto tag45add2; goto tag46add2; 

	goto tag31add2; goto tag32add2; goto tag33add2; goto tag34add2;
	goto tag35add2; goto tag36add2;
	
	goto tag21add2; goto tag22add2; goto tag23add2; goto tag24add2;
	goto tag25add2; goto tag26add2;
	
	goto tag112; goto tag11add2; goto tag11;
	goto tag122; goto tag12add2; goto tag12;
	goto tag132; goto tag13add2; goto tag13;
	goto tag142; goto tag14add2; goto tag14;
	goto tag152; goto tag15add2; goto tag15;
	goto tag162; goto tag16add2; 
}

int Inv163(WORD32 *C, WORD32 *A)
{
	WORD32 k;
	WORD32 Penta163[6];

	Penta163[0] = 0x00000107;
	Penta163[1] = 0x00000000;
	Penta163[2] = 0x00000000;
	Penta163[3] = 0x00000000;
	Penta163[4] = 0x00000000;
	Penta163[5] = 0x00000008;

	k = AlmostInv163(C, A, Penta163);
	Divide163(C, k);
	return 1;
}

