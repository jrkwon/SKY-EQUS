#include "Ecelgamal.h"

int i2d_EC_ELGAMAL(EC_ELGAMAL *a, unsigned char **pp)
{
	BIGNUM *num[5];
	ASN1_INTEGER bs;
	unsigned int j, i, tot, t, len, max=0;
	unsigned char *p;
	
	if(a == NULL) return 0;
	
	num[0] = BN_new(); num[1] = BN_new();
	num[2] = BN_new(); num[3] = BN_new();
	num[4] = BN_new();

	num[0]->neg = 0;   num[1]->neg = 0;
	num[2]->neg = 0;   num[3]->neg = 0;
	num[4]->neg = 0;

	BN_set_word(num[0], 6);
	Field2Bn(num[1], a->Ph.x);
	Field2Bn(num[2], a->Ph.y);
	Field2Bn(num[3], a->Pr.x);
	Field2Bn(num[4], a->Pr.y);
	
	bs.type = V_ASN1_INTEGER;
	tot = 0;

	for(i=0; i<5; i++)
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

	for(i=0; i<5; i++)
	{
		bs.length=BN_bn2bin(num[i], bs.data);
		i2d_ASN1_INTEGER(&bs, &p);
	}

	Free(bs.data);
	*pp = p;
	BN_clear_free(num[0]);
	BN_clear_free(num[1]);
	BN_clear_free(num[2]);
	BN_clear_free(num[3]);
	BN_clear_free(num[4]);
	return t;
}

EC_ELGAMAL *d2i_EC_ELGAMAL(EC_ELGAMAL **v, unsigned char **pp, long length)
{
	BIGNUM *num[5];
	ASN1_INTEGER *bs=NULL;
	M_ASN1_D2I_vars(v, EC_ELGAMAL *, EC_ELGAMAL_new);

	/* initialize */
	num[0] = num[1] = num[2] = num[3] = num[4] = NULL;

	M_ASN1_D2I_Init();
	M_ASN1_D2I_start_sequence();
	M_ASN1_D2I_get(bs, d2i_ASN1_INTEGER);
	if((num[0] = BN_bin2bn(bs->data, bs->length, NULL))== NULL) goto err_bn;
	M_ASN1_D2I_get(bs, d2i_ASN1_INTEGER);
	if((num[1] = BN_bin2bn(bs->data, bs->length, NULL))== NULL) goto err_bn;
	M_ASN1_D2I_get(bs, d2i_ASN1_INTEGER);
	if((num[2] = BN_bin2bn(bs->data, bs->length, NULL))== NULL) goto err_bn;
	M_ASN1_D2I_get(bs, d2i_ASN1_INTEGER);
	if((num[3] = BN_bin2bn(bs->data, bs->length, NULL))== NULL) goto err_bn;
	M_ASN1_D2I_get(bs, d2i_ASN1_INTEGER);
	if((num[4] = BN_bin2bn(bs->data, bs->length, NULL))== NULL) goto err_bn;

	Bn2Field(ret->Ph.x, num[1], 6);
	Bn2Field(ret->Ph.y, num[2], 6);
	Bn2Field(ret->Pr.x, num[3], 6);
	Bn2Field(ret->Pr.y, num[4], 6);

	M_ASN1_INTEGER_free(bs);
	BN_clear_free(num[0]); BN_clear_free(num[1]);
	BN_clear_free(num[2]); BN_clear_free(num[3]);
	BN_clear_free(num[4]);
	num[0] = num[1] = num[2] = num[3] = NULL;
	num[4] = NULL;

	M_ASN1_D2I_Finish_2(v);
err_bn:
err:
	if((ret != NULL) && ((v == NULL) || (*v != ret))) EC_ELGAMAL_free(ret);
	if(bs != NULL) M_ASN1_INTEGER_free(bs);
	if(num[0]) BN_clear_free(num[0]); 
	if(num[1]) BN_clear_free(num[1]); 
	if(num[2]) BN_clear_free(num[2]); 
	if(num[3]) BN_clear_free(num[3]); 
	if(num[4]) BN_clear_free(num[4]); 
	return (NULL);
}
