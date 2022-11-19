#include "bn_lcl.h"
#include "aapiclnt.h"


extern void  wip_free(void* mem);
extern void *wip_malloc(unsigned long size);

int BN_ModMulMontgomery(r,a,b,mont,ctx)
//                  val[0],aa,mont->RR,mont,ctx
BIGNUM *r,*a,*b;
BN_MONT_CTX *mont;
BN_CTX *ctx;
	{
	BIGNUM *tmp;

        tmp=ctx->bn[ctx->tos++];

	if (a == b)
		{
		if (!BN_Sqr(tmp,a,ctx)) goto err;
		}
	else
		{

	 		if (!BN_Mul(tmp,a,b)) goto err;
		}

	if (!BN_FromMontgomery(r,tmp,mont,ctx)) goto err;

	ctx->tos--;
	return(1);
err:
	return(0);
	}

#define MONT_WORD

int BN_FromMontgomery(ret,a,mont,ctx)
BIGNUM *ret;
BIGNUM *a;
BN_MONT_CTX *mont;
BN_CTX *ctx;
	{
	BIGNUM *n,*t1,*r;
	BN_ULONG *ap,*np,*rp,k,n0,v,v2;
	int al,nl,max,i,x;
	int retn=0;

	t1=ctx->bn[ctx->tos];
	r =ctx->bn[ctx->tos+1];
	
	//같음============================
	if (!BN_Copy(r,a)) goto err;
	//=================================

	n=mont->N;
//===========================같음===========================
	if (!BN_Copy(t1,a)) goto err;
	BN_MaskBits(t1,mont->ri);
	a=t1;
//===========================같음===========================

	al=a->top;
	nl=n->top;
	if ((al == 0) || (nl == 0)) { r->top=0; return(1); }

	max=(nl+al+1);

/*===========================================================================
					jjkyg 2000/02/24
	        realloc된것을 원래의  pointer에 돌려 줌..
 ===========================================================================*/
	if(bn_expand(r,(max)*BN_BITS2) == NULL) goto err;

	r->neg=a->neg^n->neg;
	ap=a->d;
	np=n->d;
	rp=r->d;

//===========================================================================================
//                          RP !=r->d임 이상함    
//                          r->d 값이 이상함 
//===========================================================================================
    

	for (i=r->top; i<max; i++)//because relloac
		r->d[i]=0;

	r->top=max;
	n0=mont->n0;

	for (i=0; i<nl; i++)
	{
		k=(rp[0]*n0)&BN_MASK2;

		v=bn_mul_add_word(rp,np,nl,k);

		for (x=nl; v; x++)
		{
			v2=rp[x];
			v2+=v;
			rp[x]=v2;
			v=((v2&BN_MASK2) < v)?1:0;
		}
		rp++;
	}

	while (r->d[r->top-1] == 0)
		r->top--;

	BN_Rshift(ret,r,mont->ri);

	if (BN_Ucmp(ret,mont->N) >= 0)
		{
		bn_qsub(ret,ret,mont->N);
		}
	retn=1;
err:
	return(retn);
	}

BN_MONT_CTX *BN_MONT_CTX_New()
	{
	BN_MONT_CTX *ret;

	if ((ret=(BN_MONT_CTX *)wip_malloc(sizeof(BN_MONT_CTX))) == NULL)
		return(NULL);
	ret->ri=0;
	ret->RR=BN_New();
	ret->N=BN_New();
	ret->Ni=NULL;
	if ((ret->RR == NULL) || (ret->N == NULL))
		{
		BN_MONT_CTX_Free(ret);
		return(NULL);
		}
	return(ret);
	}

void BN_MONT_CTX_Free(mont)
BN_MONT_CTX *mont;
	{
	if (mont->RR != NULL) BN_Free(mont->RR);
	if (mont->N != NULL) BN_Free(mont->N);
	if (mont->Ni != NULL) BN_Free(mont->Ni);
	wip_free(mont);
	}

int BN_MONT_CTX_Set(mont,mod,ctx)
BN_MONT_CTX *mont;
BIGNUM *mod;
BN_CTX *ctx;
	{
	BIGNUM *Ri=NULL,*R=NULL;

	if (mont->RR == NULL) mont->RR=BN_New();
	if (mont->N == NULL)  mont->N=BN_New();

	R=mont->RR;
	BN_Copy(mont->N,mod);

{
	BIGNUM tmod;
	BN_ULONG buf[2];

	mont->ri=(BN_NumBits(mod)+(BN_BITS2-1))/BN_BITS2*BN_BITS2;
	BN_Lshift(R,BN_ValueOne(),BN_BITS2);

	buf[0]=mod->d[0];
	buf[1]=0;

	tmod.d=buf;
	tmod.top=1;
	tmod.max=mod->max;
	tmod.neg=mod->neg;

	if ((Ri=BN_ModInverse(R,&tmod,ctx)) == NULL) goto err;
	BN_Lshift(Ri,Ri,BN_BITS2);
	bn_qsub(Ri,Ri,BN_ValueOne());
	BN_Div(Ri,NULL,Ri,&tmod,ctx);
	mont->n0=Ri->d[0];
	BN_Free(Ri);
}
	BN_Lshift(mont->RR,BN_ValueOne(),mont->ri*2);
	BN_Div(NULL,mont->RR,mont->RR,mont->N,ctx);

	return(1);
err:
	return(0);
	}

