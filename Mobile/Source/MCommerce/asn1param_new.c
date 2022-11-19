#include "Ecc.h"

#define NID_c2pnb163v1 213
static unsigned char lvalues[3] = {0x03,0x00,0x01};
static ASN1_OBJECT oid = {"c2pnb163v1","C2PNB163V1",NID_c2pnb163v1,3,lvalues,0};

int i2d_ECCparams(ECC *ecc, unsigned char **pp)
{
    ASN1_OBJECT *o=NULL;
    int t,ret=-1;
    unsigned char *p;

    if(ecc == NULL) return (0);

    if((o=&oid) == NULL)
    {
        goto err;
    }
	t=i2d_ASN1_OBJECT(o, NULL);
    if(pp == NULL)
    {
        return t;
    }
    p = *pp;
    i2d_ASN1_OBJECT(o, &p);
	ret = t;
err:
    *pp=p;
    return(ret);
}

ECC *d2i_ECCparams(ECC **ecc, unsigned char **pp, long length)
{
	unsigned char *q, *p, *max;
	int inf, tag, xclass;
	long len;
	ASN1_OBJECT *o=NULL;
	M_ASN1_D2I_vars(ecc, ECC *, ECC_new);
	M_ASN1_D2I_Init();

	p = *pp;
	q = p;
	max = (p+length);

	inf = ASN1_get_object(&q, &len, &tag, &xclass, length);
	if(inf & 0x80) goto err;
	M_ASN1_D2I_begin();

	if(tag == V_ASN1_OBJECT)
	{
		M_ASN1_D2I_get(o, d2i_ASN1_OBJECT);
//		if(ECC_CURVE163_V0 != OBJ_obj2nid(o)) goto err;
		ASN1_OBJECT_free(o);
		M_ASN1_D2I_Finish_2(ecc);
	}else goto err;

err:
	if ((ret != NULL) && ((ecc == NULL) || (*ecc != ret)))
	{
		ECC_free(ret);
	}
	if (o != NULL) ASN1_OBJECT_free(o);
	return(NULL);
}


