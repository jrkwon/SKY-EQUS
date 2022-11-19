#include "bn_lcl.h"

static char *Hex="0123456789ABCDEF";


int BN_Ascii2Bn(BIGNUM **bn,char *a)

	{
	BIGNUM *ret=NULL;
	BN_ULONG l=0;
	int neg=0,h,m,i,j,k,c;
	int num;

	if ((a == NULL) || (*a == '\0')) return(0);

	if (*a == '-') { neg=1; a++; }

	for (i=0; isxdigit((int)a[i]); i++)
		;

	num=i+neg;
	if (bn == NULL) return(num);

	if (*bn == NULL)
		{
		if ((ret=BN_New()) == NULL) return(0);
		}
	else
		{
		ret= *bn;
		BN_Zero(ret);
		}

	if (bn_expand_wap(ret,i*4) == NULL) goto err;

	j=i;
	m=0;
	h=0;
	while (j > 0)
		{
		m=((BN_BYTES*2) <= j)?(BN_BYTES*2):j;
		l=0;
		for (;;)
			{
			c=a[j-m];
			if ((c >= '0') && (c <= '9')) k=c-'0';
			else if ((c >= 'a') && (c <= 'f')) k=c-'a'+10;
			else if ((c >= 'A') && (c <= 'F')) k=c-'A'+10;
			else k=0;
			l=(l<<4)|k;

			if (--m <= 0)
				{
				ret->d[h++]=l;
				break;
				}
			}
		j-=(BN_BYTES*2);
		}
	ret->top=h;
	bn_fix_top(ret);
	ret->neg=neg;

	*bn=ret;
	return(num);
err:
	if (*bn == NULL) BN_Free(ret);
	return(0);
	}
