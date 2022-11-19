#include "Ecdh.h"
#define BUF_MAX_SIZE 128

/********************************************************************
    Function: ECDH_compute_key(key, pub_key, ecc)
    Input:    key -- a unsigned char pointer of storage saved for
              the result
              pub_key -- partner's public key(one point)
              ecc -- my private key
    Output:   positive value -- successfully run and return the 
              length of the key(secret value)
              negative value(-1) -- fail to compute the key value
    Effects:  <none>

    Description: This routine is used to compute the common secret
      value. Timing attack can be protected by default. If you don't
      want to protect the timing attack, define macro(NO_TIMING_ATTA
      CK). Maybe performance will be good.

      (Mathematical Operation)
      key(secret value) := ecc->priv_key * pub_key
                           ^^^^^^^^^^^^^^^^^^^^^^^
                           Point Multiplication
********************************************************************/

int ECDH_compute_key(unsigned char *key, Point *pub_key, ECC *ecc)
{
	Point P;
	BIGNUM *temp=NULL;
	int ret = -1;

#ifndef NO_TIMING_ATTACK
	if(!generate_blinding_pair(ecc)) goto keyErr;
	if(!Eadd(pub_key, pub_key, &Vi, ecc->curve)) goto keyErr;
#endif 

	/* if P is the infinite point, goto keyErr */
	if(!PMul(&P, pub_key, ecc->priv_key, ecc->order, ecc->curve)) goto keyErr;
	
#ifndef NO_TIMING_ATTACK
	if(!Esub(pub_key, pub_key, &Vi, ecc->curve)) goto keyErr;
	if(!Esub(&P, &P, &Vf, ecc->curve)) goto keyErr;
#endif

	if((temp = BN_new()) == NULL) goto keyErr;
	Field2Bn(temp, P.x);
	ret = BN_bn2bin(temp, key);

keyErr:
	if(temp) BN_clear_free(temp);
	return ret;
}

int ANSI_X9_63_KDF(unsigned char *dst, unsigned long keylen,\
				   unsigned char *secret, unsigned long size,\
				   unsigned char *shared, unsigned long slen)
{
	unsigned long count, temp;
	unsigned long bound, datalen;
	unsigned char buffer[BUF_MAX_SIZE];
	/* message digest */
	unsigned char md[SHA_DIGEST_LENGTH]; 
	unsigned char counter[4];
	unsigned long i;

	if(dst==NULL) goto KDFErr;
	if(secret==NULL) goto KDFErr;

	count = 1;
	datalen = size+4;
	if(shared) datalen += slen;

	/* shared Info is So large */
	if(datalen>BUF_MAX_SIZE) goto KDFErr;

	bound = (keylen/SHA_DIGEST_LENGTH);
	if((keylen%SHA_DIGEST_LENGTH)) bound++;
	
	strncpy((char *)buffer, (const char *)secret, size);
	if(shared) strncpy((char *)&buffer[size+4], (const char *)shared, slen);

	for(i=0; i<bound; i++)
	{
		temp = count;
		counter[3] = (unsigned char)(temp%256);
		temp = (temp>>8);
		counter[2] = (unsigned char)(temp%256);
		temp = (temp>>8);
		counter[1] = (unsigned char)(temp%256);
		temp = (temp>>8);
		counter[0] = (unsigned char)(temp);
		
		buffer[size] = counter[0];
		buffer[size+1] = counter[1];
		buffer[size+2] = counter[2];
		buffer[size+3] = counter[3];

		SHA1(buffer, datalen, md);

		if(i==bound-1 && keylen%SHA_DIGEST_LENGTH)
		{
				strncpy((char *)&dst[SHA_DIGEST_LENGTH*i], (const char *)md,\
					keylen%SHA_DIGEST_LENGTH);
		}else strncpy((char *)&dst[SHA_DIGEST_LENGTH*i], (const char *)md, SHA_DIGEST_LENGTH);
		count++;
	}

	return 1;
KDFErr:
	return 0;
}

