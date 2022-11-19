#include "bn_lcl.h"
#include "comdef.h"
#include "aapiclnt.h"

extern VOID    CLNTa_log (UINT8 viewId, INT16 logNo, const CHAR *format, ...);

int BN_ModExp(r,a,p,m,ctx)
BIGNUM *r;
BIGNUM *a;
BIGNUM *p;
BIGNUM *m;
BN_CTX *ctx;
{
	int ret=0;
	if (BN_IsOdd(m)) { ret=BN_ModExpMont(r,a,p,m,ctx); }
	else {  }

	return(ret);
}

int BN_ModExpMont(r,a,p,m,ctx)
BIGNUM *r;
BIGNUM *a;
BIGNUM *p;
BIGNUM *m;
BN_CTX *ctx;
{
	int i,j,bits,ret=0,wstart,wend,window,wvalue;
	int start=1;
	BIGNUM *d,*aa;
	BIGNUM *val[16];
	BN_MONT_CTX *mont=NULL;

	if (!(m->d[0] & 1))
		{
		return(0);
		}
	d=ctx->bn[ctx->tos++];
	bits=BN_NumBits(p);
	if (bits == 0)
		{
		BN_One(r);
		return(1);
		}

	if ((mont=BN_MONT_CTX_New()) == NULL) goto err;
	if (!BN_MONT_CTX_Set(mont,m,ctx)) goto err;
//================================================================================================
//================================================================================================
	val[0]=BN_New();
	
	if (BN_Ucmp(a,m) >= 0)
	{
		//여긴 안들어옴...
		BN_Div(NULL,val[0],a,m,ctx);
		aa=val[0];
	}
	else
		aa=a;

	/*=======================================================================
	  1. val 비교  : 같음 
	  2. aa  비교  : 같음 
	  3. ctx 비교 
	  4. mont비교  : 같음 
	 =======================================================================*/
	if (!BN_ToMontgomery(val[0],aa,mont,ctx)) goto err;

	if (!BN_ModMulMontgomery(d,val[0],val[0],mont,ctx)) goto err;

//===========================================================================
	if (bits <= 17)
		window=1;
	else if (bits >= 256)
		window=5;
	else if (bits >= 128)
		window=4;
	else
		window=3;

	j=1<<(window-1);
//========================여긴 안들어옴======================================
	for (i=1; i<j; i++)
		{
		val[i]=BN_New();
		if (!BN_ModMulMontgomery(val[i],val[i-1],d,mont,ctx))
			goto err;
		}
//========================여긴 안들어옴======================================
	for (; i<16; i++)
		val[i]=NULL;

	start=1;
	wvalue=0;
	wstart=bits-1;
	wend=0;
			 
        if (!BN_ToMontgomery(r,BN_ValueOne(),mont,ctx)) goto err;
//값은 같음

	for (;;)
	{//for start
		if (BN_IsBitSet(p,wstart) == 0)
			{
			if (!start)
				if (!BN_ModMulMontgomery(r,r,r,mont,ctx))
				{
					goto err;
				}
			if (wstart == 0) break;
			wstart--;
			continue;
			}
		j=wstart;
		wvalue=1;
		wend=0;
		for (i=1; i<window; i++)
			{
			if (wstart-i < 0) break;
			if (BN_IsBitSet(p,wstart-i))
				{
				wvalue<<=(i-wend);
				wvalue|=1;
				wend=i;
				}
			}

		j=wend+1;
		if (!start)
		{
			for (i=0; i<j; i++)
			{
				if (!BN_ModMulMontgomery(r,r,r,mont,ctx))
				{
					goto err;
				}
			}
		}
		
		if (!BN_ModMulMontgomery(r,r,val[wvalue>>1],mont,ctx))
		{
			goto err;
		}

		wstart-=wend+1;
		wvalue=0;
		start=0;
		if (wstart < 0) break;
	}//for end

	BN_FromMontgomery(r,r,mont,ctx);

	ret=1;
err:
	if (mont != NULL) BN_MONT_CTX_Free(mont);
	ctx->tos--;
	for (i=0; i<16; i++)
		if (val[i] != NULL) BN_ClearFree(val[i]);
	return(ret);
}
