#ifndef BN_H
#define BN_H

#define RECP_MUL_MOD
#define MONT_MUL_MOD

#undef SIXTY_FOUR_BIT_LONG
#undef SIXTY_FOUR_BIT
#define THIRTY_TWO_BIT
#undef SIXTEEN_BIT
#undef EIGHT_BIT

#ifdef THIRTY_TWO_BIT
#define BN_ULLONG	unsigned long long
#define BN_ULONG	unsigned long
#define BN_LONG		long
#define BN_BITS		64
#define BN_BYTES	4
#define BN_BITS2	32
#define BN_BITS4	16
#define BN_MASK2	(0xffffffffL)
#define BN_MASK2l	(0xffff)
#define BN_MASK2h1	(0xffff8000L)
#define BN_MASK2h	(0xffff0000L)
#define BN_TBIT		(0x80000000L)
#endif

#ifdef SIXTEEN_BIT
#ifndef BN_DIV2W
#define BN_DIV2W
#endif
#define BN_ULLONG	unsigned long
#define BN_ULONG	unsigned short
#define BN_LONG		short
#define BN_BITS		32
#define BN_BYTES	2
#define BN_BITS2	16
#define BN_BITS4	8
#define BN_MASK2	(0xffff)
#define BN_MASK2l	(0xff)
#define BN_MASK2h1	(0xff80)
#define BN_MASK2h	(0xff00)
#define BN_TBIT		(0x8000)
#endif

#define BN_DEFAULT_BITS	1280

#ifdef BIGNUM
#undef BIGNUM
#endif

typedef struct bignum_st {
	BN_ULONG *d;
	int top;
	int max;
	int neg;
} BIGNUM;

#define BN_CTX_NUM	12
typedef struct bignum_ctx {
	int tos;
	BIGNUM *bn[BN_CTX_NUM+1];
} BN_CTX;

typedef struct bn_mont_ctx_st {
        int ri;
        BIGNUM *RR;
        BIGNUM *N;
        BIGNUM *Ni;
	BN_ULONG n0;
} BN_MONT_CTX;

#define BN_ToMontgomery(r,a,mont,ctx)	BN_ModMulMontgomery(\
	r,a,(mont)->RR,(mont),ctx)

#define BN_NumBytes(a)	((BN_NumBits(a)+7)/8)
#define BN_IsWord(a,w)	(((a)->top == 1) && ((a)->d[0] == (BN_ULONG)(w)))
#define BN_IsZero(a)	(((a)->top <= 1) && ((a)->d[0] == (BN_ULONG)0))
#define BN_IsOne(a)	(BN_IsWord((a),1))
#define BN_IsOdd(a)	((a)->d[0] & 1)
#define BN_One(a)	(BN_SetWord((a),1))
#define BN_Zero(a)	(BN_SetWord((a),0))

#define bn_fix_top(a) \
	{ \
	BN_ULONG *fix_top_l; \
	for (fix_top_l= &((a)->d[(a)->top-1]); (a)->top > 0; (a)->top--) \
		if (*(fix_top_l--)) break; \
	}

#define bn_expand_wap(n,b) ((((b)/BN_BITS2) <= (n)->max)?(n):bn_expand_new((n),(b)))

BIGNUM *BN_ValueOne(void);
BN_CTX *BN_CTX_New(void);
void	BN_CTX_Free(BN_CTX *c);
int	BN_NumBits(BIGNUM *a);
int	BN_NumBitsWord(BN_ULONG);
BIGNUM *BN_New(void);
void	BN_ClearFree(BIGNUM *a);
BIGNUM *BN_Copy(BIGNUM *a, BIGNUM *b);
BIGNUM *BN_Bin2Bn(unsigned char *s,int len,BIGNUM *ret);
int	BN_Bn2Bin(BIGNUM *a, unsigned char *to);
int	BN_Sub(BIGNUM *r, BIGNUM *a, BIGNUM *b);
void	bn_qsub(BIGNUM *r, BIGNUM *a, BIGNUM *b);
void	bn_qadd(BIGNUM *r, BIGNUM *a, BIGNUM *b);
int	BN_Add(BIGNUM *r, BIGNUM *a, BIGNUM *b);
int	BN_Mod(BIGNUM *rem, BIGNUM *m, BIGNUM *d, BN_CTX *ctx);
int	BN_Div(BIGNUM *dv, BIGNUM *rem, BIGNUM *m, BIGNUM *d, BN_CTX *ctx);
int	BN_Mul(BIGNUM *r, BIGNUM *a, BIGNUM *b);
int	BN_Sqr(BIGNUM *r, BIGNUM *a,BN_CTX *ctx);
BN_ULONG BN_ModWord(BIGNUM *a, unsigned long w);
BN_ULONG BN_DivWord(BIGNUM *a, unsigned long w);
int	BN_AddWord(BIGNUM *a, unsigned long w);
int	BN_SetWord(BIGNUM *a, unsigned long w);
unsigned long BN_GetWord(BIGNUM *a);
int	BN_Cmp(BIGNUM *a, BIGNUM *b);
void	BN_Free(BIGNUM *a);
int	BN_IsBitSet(BIGNUM *a, int n);
int	BN_Lshift(BIGNUM *r, BIGNUM *a, int n);
int	BN_Lshift1(BIGNUM *r, BIGNUM *a);
int	BN_ModExp(BIGNUM *r, BIGNUM *a, BIGNUM *p, BIGNUM *m,BN_CTX *ctx);
int	BN_ModExpMont(BIGNUM *r, BIGNUM *a, BIGNUM *p, BIGNUM *m,BN_CTX *ctx);
int	BN_ModExpRecp(BIGNUM *r, BIGNUM *a, BIGNUM *p, BIGNUM *m,BN_CTX *ctx);
int	BN_MaskBits(BIGNUM *a,int n);
int	BN_ModMulReciprocal(BIGNUM *r, BIGNUM *x, BIGNUM *y, BIGNUM *m, 
	BIGNUM *i, int nb, BN_CTX *ctx);
int	BN_ModMul(BIGNUM *ret, BIGNUM *a, BIGNUM *b, BIGNUM *m,
	BN_CTX *ctx);
int	BN_Reciprocal(BIGNUM *r, BIGNUM *m, BN_CTX *ctx);
int	BN_Rshift(BIGNUM *r, BIGNUM *a, int n);
int	BN_Rshift1(BIGNUM *r, BIGNUM *a);
void	BN_Clear(BIGNUM *a);
BIGNUM *bn_expand_new(BIGNUM *b, int bits);
BIGNUM *BN_Dup(BIGNUM *a);
int	BN_Ucmp(BIGNUM *a, BIGNUM *b);
int	BN_SetBit(BIGNUM *a, int n);
int	BN_ClearBit(BIGNUM *a, int n);
char *	BN_Bn2Ascii(BIGNUM *a);
int 	BN_Ascii2Bn(BIGNUM **a,char *str);
BIGNUM *BN_ModInverse(BIGNUM *a, BIGNUM *n,BN_CTX *ctx);

BN_ULONG bn_mul_add_word(BN_ULONG *rp, BN_ULONG *ap, int num, BN_ULONG w);
BN_ULONG bn_mul_word(BN_ULONG *rp, BN_ULONG *ap, int num, BN_ULONG w);
void     bn_sqr_words(BN_ULONG *rp, BN_ULONG *ap, int num);
BN_ULONG bn_div64(BN_ULONG h, BN_ULONG l, BN_ULONG d);

BN_MONT_CTX *BN_MONT_CTX_New(void );
int BN_ModMulMontgomery(BIGNUM *r,BIGNUM *a,BIGNUM *b,BN_MONT_CTX *mont,
	BN_CTX *ctx);
int BN_FromMontgomery(BIGNUM *r,BIGNUM *a,BN_MONT_CTX *mont,BN_CTX *ctx);
void BN_MONT_CTX_Free(BN_MONT_CTX *mont);
int BN_MONT_CTX_Set(BN_MONT_CTX *mont,BIGNUM *modulus,BN_CTX *ctx);

#endif

