#include "Ecdsa.h"

int i2d_ECDSA_SIG(ECDSA_SIG *a, unsigned char **pp)
{
	BIGNUM *num[2];
	ASN1_INTEGER bs;
	unsigned int j, i, tot, t, len, max=0;
	unsigned char *p;
	
	if(a == NULL || a->r == NULL || a->s == NULL) return 0;
	
	num[0] = a->r;
	num[1] = a->s;
	
	bs.type = V_ASN1_INTEGER;
	tot = 0;

	for(i=0; i<2; i++)
	{
		j = BN_num_bits(num[i]);
		len = ((j == 0)?0:((j/8)+1));
		if(len > max) max=len;
		len=ASN1_object_size(0, len, V_ASN1_INTEGER);
		tot += len;
	}

	t = ASN1_object_size(1, tot, V_ASN1_SEQUENCE);
	if(pp == NULL) return(t);

	p = *pp;
	ASN1_put_object(&p, 1, tot, V_ASN1_SEQUENCE, V_ASN1_UNIVERSAL);

	bs.data=(unsigned char *)Malloc(max+4);
	if(bs.data == NULL) return -1;

	for(i=0; i<2; i++)
	{
		bs.length=BN_bn2bin(num[i], bs.data);
		i2d_ASN1_INTEGER(&bs, &p);
	}

	Free(bs.data);
	*pp = p;
	return t;
}

ECDSA_SIG *d2i_ECDSA_SIG(ECDSA_SIG **v, unsigned char **pp, long length)
{
	ASN1_INTEGER *bs=NULL;
	M_ASN1_D2I_vars(v, ECDSA_SIG *, ECDSA_SIG_new);

	M_ASN1_D2I_Init();
	M_ASN1_D2I_start_sequence();
	M_ASN1_D2I_get(bs, d2i_ASN1_INTEGER);
	if((ret->r = BN_bin2bn(bs->data, bs->length, ret->r))== NULL) goto err_bn;
	M_ASN1_D2I_get(bs, d2i_ASN1_INTEGER);
	if((ret->s = BN_bin2bn(bs->data, bs->length, ret->s))== NULL) goto err_bn;
	M_ASN1_INTEGER_free(bs);

	M_ASN1_D2I_Finish_2(v);
err_bn:
err:
	if((ret != NULL) && ((v == NULL) || (*v != ret))) ECDSA_SIG_free(ret);
	if(bs != NULL) M_ASN1_INTEGER_free(bs);
	return (NULL);
}
