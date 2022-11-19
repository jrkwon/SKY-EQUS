#include "Ecc.h"

int i2d_ECCPublicKey(ECC *ecc, unsigned char **pp)
{
    int t;
    unsigned char *temp;
	unsigned char *p;
	ASN1_OCTET_STRING str;

    if(ecc==NULL) return 0;

    t = OctetSizeOfPoint(0);
	str.length = t;
	t = ASN1_object_size(0, t, V_ASN1_OCTET_STRING);
    if(pp==NULL)
    {
        return t;
    }
    
	temp = Point2Octet(ecc->pub_key, 0);
	str.data = temp;

	p = *pp;
	i2d_ASN1_OCTET_STRING(&str, &p);	

	*pp = p;
    Free(temp);
    return t;
}


ECC *d2i_ECCPublicKey(ECC **ecc, unsigned char **pp, long length)
{
	ASN1_OCTET_STRING *str = NULL;
	M_ASN1_D2I_vars(ecc, ECC *, ECC_new);

    if((ret->pub_key = Point_new())==NULL) goto err;

	M_ASN1_D2I_Init();
	M_ASN1_D2I_begin();
	M_ASN1_D2I_get(str, d2i_ASN1_OCTET_STRING);
    if(!Octet2Point(ret->pub_key, str->data, str->length))
    {
        goto err;
    }
	
	M_ASN1_OCTET_STRING_free(str);
	M_ASN1_D2I_Finish_2(ecc);
err:

	if((ret!=NULL) && ((ecc==NULL)||(*ecc!=ret))) ECC_free(ret);
	if(str!=NULL) M_ASN1_OCTET_STRING_free(str);
    return NULL;
}













