#include "Ecc.h"

int i2d_ECCPrivateKey(ECC *ecc, unsigned char **pp)
{
	BIGNUM *num[2];
	ASN1_INTEGER bs;
	ASN1_OCTET_STRING str;

	int v1=0, v2=0;
	unsigned int j, tot, t, len;
	unsigned char *p;

	num[0] = NULL; num[1] = NULL;
	bs.data = NULL; str.data = NULL;

	if(ecc==NULL) return 0;
	if(ecc->priv_key==NULL) return 0;

	if((num[0] = BN_new()) == NULL) goto err;
	if((num[1] = BN_new()) == NULL) goto err;

	BN_set_word(num[0], 1);
	BN_copy(num[1], ecc->priv_key);

	bs.type=V_ASN1_INTEGER;
	tot=0;

	/* version INTEGER { ecPrivkeyVer1(1) } (ecPrivKeyVer1) */
	j=BN_num_bits(num[0]);
	bs.length = len = (j+7)/8;
	len = ASN1_object_size(0, len, V_ASN1_INTEGER);
	tot += len;

	/* privateKey OCTET STRING */
	j=BN_num_bits(num[1]);
	str.length = len = (j+7)/8;
	len = ASN1_object_size(0, len, V_ASN1_OCTET_STRING);
	tot += len;

	/* parameters [0] Parameters OPTIONAL */
	v1 = len = i2d_ECCparams(ecc, NULL);
	tot += ASN1_object_size(1, len, 0);
	/* publicKey [1] BIT STRING OPTIONAL */
	v2 = len = i2d_ECCPublicKey(ecc, NULL);
	tot += ASN1_object_size(1, len, 1);

	t=ASN1_object_size(1, tot, V_ASN1_SEQUENCE);
	if(pp==NULL)
	{
		if(num[0]) BN_clear_free(num[0]);
		if(num[1]) BN_clear_free(num[1]);
		return(t);
	}

	/* version INTEGER { ecPrivkeyVer1(1) } (ecPrivKeyVer1) */
	bs.data = (unsigned char *)Malloc(bs.length+4);
	if(bs.data == NULL) goto err;
	bs.length = BN_bn2bin(num[0], bs.data);

	/* privateKey OCTET STRING */
	str.data = (unsigned char *)Malloc(str.length+4);
	if(str.data == NULL) goto err;
	str.length = BN_bn2bin(num[1], str.data);

	p = *pp;
	ASN1_put_object(&p, 1, tot, V_ASN1_SEQUENCE, V_ASN1_UNIVERSAL);

	i2d_ASN1_INTEGER(&bs, &p);
	i2d_ASN1_OCTET_STRING(&str, &p);

	/* parameters [0] Parameters OPTIONAL */
	ASN1_put_object(&p, 1, v1, 0, V_ASN1_CONTEXT_SPECIFIC);
	i2d_ECCparams(ecc, &p);

	/* publicKey [1] BIT STRING OPTIONAL */
	ASN1_put_object(&p, 1, v2, 1, V_ASN1_CONTEXT_SPECIFIC);
	i2d_ECCPublicKey(ecc, &p);

	Free(bs.data);
	Free(str.data);
	*pp=p;
	BN_clear_free(num[0]);
	BN_clear_free(num[1]);
	return t;
err:
	if(bs.data) Free(bs.data);
	if(str.data) Free(str.data);
	if(num[0]) BN_clear_free(num[0]);
	if(num[1]) BN_clear_free(num[1]);
	return 0;
}

ECC *d2i_ECCPrivateKey(ECC **ecc, unsigned char **pp, long length)
{
	ASN1_INTEGER *bs=NULL;
	ASN1_OCTET_STRING *str=NULL;
	BIGNUM *num0=NULL;
	BIGNUM *temp=NULL;
	M_ASN1_D2I_vars(ecc, ECC *, ECC_new);

	M_ASN1_D2I_Init();
	M_ASN1_D2I_start_sequence();

	M_ASN1_D2I_get(bs, d2i_ASN1_INTEGER);
	if((num0= BN_bin2bn(bs->data, bs->length, NULL))==NULL)
	{
		goto err_bn;
	}

	M_ASN1_D2I_get(str, d2i_ASN1_OCTET_STRING);
	if((temp = BN_bin2bn(str->data, str->length, NULL))==NULL)
	{
		goto err_bn;
	}

	if(M_ASN1_next == (V_ASN1_CONTEXT_SPECIFIC | V_ASN1_CONSTRUCTED | 0))
	{
		M_ASN1_D2I_get_EXP_opt(ret, d2i_ECCparams, 0);
	}

	if(M_ASN1_next == (V_ASN1_CONTEXT_SPECIFIC | V_ASN1_CONSTRUCTED | 1))
	{
		M_ASN1_D2I_get_EXP_opt(ret, d2i_ECCPublicKey, 1);
	}else
	{
		if(ret->pub_key != NULL)
		{
			Point_free(ret->pub_key);
		}
	}

	if(ret->priv_key) BN_clear_free(ret->priv_key);
	ret->priv_key = temp;

	M_ASN1_INTEGER_free(bs);
	M_ASN1_OCTET_STRING_free(str);
	BN_clear_free(num0);
	M_ASN1_D2I_Finish_2(ecc);

err_bn:
	err:
	if((ret!=NULL) && ((ecc==NULL)||(*ecc!=ret))) ECC_free(ret);
	if(bs!=NULL) M_ASN1_INTEGER_free(bs);
	if(str!=NULL) M_ASN1_OCTET_STRING_free(str);
	if(num0) BN_clear_free(num0);
	if(temp) BN_clear_free(temp);
	return (NULL);
}
