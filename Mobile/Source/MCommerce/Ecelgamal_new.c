#include "Ecelgamal.h"

/*
 * Made by Suengchur Pyun!!
 * 
 * e-mail : pyun@postech.edu
 */

EC_ELGAMAL *EC_ELGAMAL_new(void)
{
	EC_ELGAMAL *ret=NULL;
	ret = (EC_ELGAMAL *)Malloc(sizeof(EC_ELGAMAL));
	if(ret==NULL) return NULL;
	return ret;
}

void EC_ELGAMAL_free(EC_ELGAMAL *r)
{
	if(r==NULL) return;
	Free(r);
}

EC_ELGAMAL *EC_ELGAMAL_do_encrypt(const unsigned char *M, int bytes, ECC *ecc)
{
	EC_ELGAMAL *result=NULL;
	Point Pm, Ps;
	BIGNUM *r=NULL;
	int tmp;
	/* so large input */
	if(bytes>16) goto encryptErr; 
	if(M==NULL) goto encryptErr;
	if(ecc==NULL) goto encryptErr;

	/* memory allocation */
	result = (EC_ELGAMAL *)Malloc(sizeof(EC_ELGAMAL));
	if(result==NULL) goto encryptErr;
	if((r = BN_new()) == NULL) goto encryptErr;
	
	/* Put message m into point Pm */
	if(!BN_bin2bn(M, bytes, r)) goto encryptErr;
	SetZero163(Pm.x); Bn2Field(Pm.x, r, 4);
	MakePoint(&Pm, ecc->curve);
	if(!ValidPoint(&Pm, ecc->curve)) goto encryptErr;

	/* Generate a random number r */
	tmp = BN_num_bits(ecc->order);
	if(!BN_rand(r, tmp-1, 0, 0)) goto encryptErr;

	/* Calculate Pr = rB */
	if(!BrickellMul(&(result->Pr), ecc->base, r, ecc->order, ecc->curve)) goto encryptErr;

	/* Calculate Ph = Pm + rPb */
	if(!PMul(&Ps, ecc->pub_key, r, ecc->order, ecc->curve)) goto encryptErr;
	if(!Eadd(&(result->Ph), &Pm, &Ps, ecc->curve)) goto encryptErr;

	return result;
encryptErr:
	
	if(result) Free(result);
	if(r) BN_clear_free(r);
	return NULL;
}

int EC_ELGAMAL_do_decrypt(const unsigned char *M, int *bytes, EC_ELGAMAL *ctext, ECC *ecc)
{
	Point Ps, Pm;
	BIGNUM *temp=NULL;

#ifndef NO_TIMING_ATTACK
	if(!generate_blinding_pair(ecc)) goto decryptErr;
	if(!Eadd(&(ctext->Pr), &(ctext->Pr), &Vi, ecc->curve)) goto decryptErr;
#endif
	
	/* Ps = Kb * Pr = Kb * r * B */
	if(!PMul(&Ps, &(ctext->Pr), ecc->priv_key, ecc->order, ecc->curve)) goto decryptErr;

#ifndef NO_TIMING_ATTACK
	if(!Esub(&(ctext->Pr), &(ctext->Pr), &Vi, ecc->curve)) goto decryptErr;
	if(!Esub(&Ps, &Ps, &Vf, ecc->curve)) goto decryptErr;
#endif 

	/* Pm = Ph - Ps = Pm */
	if(!Esub(&Pm, &(ctext->Ph), &Ps, ecc->curve)) goto decryptErr;

	Pm.x[4] = 0;
	if((temp = BN_new()) == NULL) goto decryptErr;
	Field2Bn(temp, Pm.x);
	if(!BN_bn2bin(temp, (unsigned char *)M)) goto decryptErr;
	*bytes = BN_num_bytes(temp);

	BN_clear_free(temp);
	return 1;

decryptErr:

	if(temp) BN_clear_free(temp);
	return 0;
}

int ECC_encrypt(const unsigned char *from, int fromlen,unsigned char *to,
				int *tolen, ECC *ecc)
{
	EC_ELGAMAL *s;
	s=EC_ELGAMAL_do_encrypt(from,fromlen,ecc);
	if (s == NULL)
	{
		*tolen=0;
		return(0);
	}
	*tolen=i2d_EC_ELGAMAL(s,&to);
	EC_ELGAMAL_free(s);
	return(1);
}

int ECC_decrypt(unsigned char *from, int fromlen,unsigned char *to,\
				int *tolen, ECC *ecc)
{
	EC_ELGAMAL *s;
	int ret=0;

	s = EC_ELGAMAL_new();
	if (s == NULL)
	{
		return(ret);
	}

	if (d2i_EC_ELGAMAL(&s,&from,fromlen) == NULL)
	{
		goto err;
	}

	ret=EC_ELGAMAL_do_decrypt(to,tolen,s,ecc);
err:
	EC_ELGAMAL_free(s);
	return(ret);
}

int ECC_size2(ECC *r)
{
	int ret, i, j;
	ASN1_INTEGER bs;
	BIGNUM *big;
	unsigned char buf[4];

	i = 163;
	bs.length = (i+7)/8;
	bs.data=buf;
	bs.type=V_ASN1_INTEGER;
	buf[0]=0xff;

	i=i2d_ASN1_INTEGER(&bs, NULL);
	i *= 4 ;

	big = BN_new();
	BN_set_word(big, 163);
	j = BN_num_bits(big);
	bs.length = (j+7)/8;
	bs.data = buf;
	bs.type = V_ASN1_INTEGER;
	buf[0]=0xff;
	BN_clear_free(big);
	j=i2d_ASN1_INTEGER(&bs, NULL);

	ret=ASN1_object_size(1,i+j,V_ASN1_SEQUENCE);

	return ret;
}
