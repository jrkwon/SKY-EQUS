#include <string.h>
#include "bn.h"
#include "rsa.h"
#include "aapiclnt.h"

extern void  wip_free(void* mem);
extern void *wip_malloc(unsigned long size);

extern void DLX_RAND_Bytes( unsigned char *buf,int num);
extern void DLX_RAND_Seed( unsigned char *buf,int num);

RSA *IA_RSA_New()
{
	RSA *ret;

	ret=(RSA *)wip_malloc(sizeof(RSA));
	if (ret == NULL) {
		return(NULL);
	}

	ret->n=NULL;
	ret->e=NULL;
	ret->d=NULL;
	ret->p=NULL;
	ret->q=NULL;
	ret->dmp1=NULL;
	ret->dmq1=NULL;
	ret->iqmp=NULL;
	return(ret);
}

void IA_RSA_Free(RSA *r)
{
	if (r == NULL) return;

	if (r->n != NULL) BN_ClearFree(r->n);
	if (r->e != NULL) BN_ClearFree(r->e);
	if (r->d != NULL) BN_ClearFree(r->d);
	if (r->p != NULL) BN_ClearFree(r->p);
	if (r->q != NULL) BN_ClearFree(r->q);
	if (r->dmp1 != NULL) BN_ClearFree(r->dmp1);
	if (r->dmq1 != NULL) BN_ClearFree(r->dmq1);
	if (r->iqmp != NULL) BN_ClearFree(r->iqmp);
	wip_free(r);
}
extern VOID CLNTa_log (UINT8 viewId, INT16 logNo, const CHAR *format, ...);

int IA_RSA_PublicEncrypt(int flen,unsigned char *from,unsigned char *to,RSA *rsa)
//                          ucSecLen,            ucpSec,       cpEncSec,    pRsa
{
        BIGNUM *f=NULL,*ret=NULL;
        int i,j,k,num=0,r= -1;
        unsigned char *p;
        unsigned char *buf=NULL;
        BN_CTX *ctx=NULL;
//		int index;

        ctx=BN_CTX_New();
        if (ctx == NULL) {
			goto err;
		}

        num=BN_NumBytes(rsa->n);

#ifdef debug
		CLNTa_log(1,0," IA RSA num %d",num);
#endif//#ifdef debug

        if (flen > (num-11)) {
                goto err;
        }
 
        buf=(unsigned char *)wip_malloc(num);

#ifdef debug
		CLNTa_log(1,0,"allocate number %d",num);
#endif//#ifdef debug
//===================================================================================
		//여기 
        if (buf == NULL)
                {
                goto err;
#ifdef debug
				CLNTa_log(1,0,"allocate error");
#endif//#ifdef debug
                }
        p=(unsigned char *)buf;

        *(p++)=0;
        *(p++)=2;

        j=num-3-flen;
 
        DLX_RAND_Bytes(p,j);
        for (i=0; i<j; i++) {
                if (*p == '\0')
                        do      {
                                DLX_RAND_Bytes(p,1);
                                } while (*p == '\0');
                p++;
        }

        *(p++)='\0';

        memcpy(p,from,(unsigned int)flen);
        f=BN_New();
        ret=BN_New();
        if ((f == NULL) || (ret == NULL)) {
			goto err;
		}

        if (BN_Bin2Bn(buf,num,f) == NULL) { 
			goto err;
		}

#ifdef debug
		CLNTa_log(1,0," BN_MOdEXP");
#endif//#ifdef debug

        if (!BN_ModExp(ret,f,rsa->e,rsa->n,ctx)) {
			goto err;
		}
//값이 틀림
//CLNTa_log(1,0,"RESULT ret value = %lx %x  %x  %x",*(ret->d),ret->top,ret->max,ret->neg);     

        j=BN_NumBytes(ret);

        i=BN_Bn2Bin(ret,&(to[num-j]));
        for (k=0; k<(num-i); k++)
                to[k]=0;

        r=num;
err:
        if (ctx != NULL) BN_CTX_Free(ctx);
        if (f != NULL) BN_Free(f);
        if (ret != NULL) BN_Free(ret);
        if (buf != NULL)
                {
                memset(buf,0,num);
                wip_free(buf);
                }
        return(r);
        }

