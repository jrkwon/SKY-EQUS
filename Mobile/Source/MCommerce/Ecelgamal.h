#ifndef HEADER_ECELGAMAL_H
#define HEADER_ECELGAMAL_H

#include "Ecc.h"

/*
 * Made by Suengchur Pyun!!
 * 
 * e-mail : pyun@postech.edu
 */

#ifdef  __cplusplus
extern "C" {
#endif

typedef struct _ec_elgamal
{
	Point Pr;
	Point Ph;
}EC_ELGAMAL;

EC_ELGAMAL *EC_ELGAMAL_new(void);
void EC_ELGAMAL_free(EC_ELGAMAL *r);

EC_ELGAMAL *EC_ELGAMAL_do_encrypt(const unsigned char *M, int bytes, ECC *ecc);
int EC_ELGAMAL_do_decrypt(const unsigned char *M, int *bytes, EC_ELGAMAL *ctext, ECC *ecc);

int ECC_encrypt(const unsigned char *from, int fromlen,
				unsigned char *to, int *tolen, ECC *ecc);
int ECC_decrypt(unsigned char *from, int fromlen,
				unsigned char *to, int * tolen, ECC *ecc);

int i2d_EC_ELGAMAL(EC_ELGAMAL *a, unsigned char **pp);
EC_ELGAMAL *d2i_EC_ELGAMAL(EC_ELGAMAL **v, unsigned char **pp, long length);

int ECC_size2(ECC *r);

#ifdef  __cplusplus
}
#endif
#endif

