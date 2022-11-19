#ifndef HEADER_ECC_H
#define HEADER_ECC_H

#include "Group163.h"
#include "rand.h"
#include "sha.h"
#include "asn1.h"
#include "asn1_mac.h"
#include "objects.h"

/*
 * Made by Suengchur Pyun!!
 * 
 * e-mail : pyun@postech.edu
 */

//#undef NO_TIMING_ATTACK
#define NO_TIMING_ATTACK

#ifdef  __cplusplus
extern "C" {
#endif

//#define ECC_CURVE163_V0 NID_ad_OCSP
extern Point Vi, Vf;

typedef struct ecc_st ECC;

struct ecc_st
{
	Curve *curve;
	Point *base;
	BIGNUM *order;
	BIGNUM *priv_key;
	Point *pub_key;
	WORD32 cofactor;
};

ECC *ECC_new(void);
void ECC_free(ECC *);

int ECC_generate_key(ECC *A);
int ECC_public_key_check(Point *pub, ECC *ecc);

int Curve_K_163(ECC *ecc);
int Curve_5(ECC *ecc);
void Curve_B_163(ECC *ecc);
int AnomalousConditionHold(ECC *ecc);
int MOVConditionHold(ECC *ecc);

/* for timing attack */
#ifndef NO_TIMING_ATTACK
int generate_blinding_pair(ECC *ecc);
void blinding_init();
#endif

int i2d_ECCPublicKey(ECC *ecc, unsigned char **pp);
ECC *d2i_ECCPublicKey(ECC **ecc, unsigned char **p, long length);

int i2d_ECCPrivateKey(ECC *ecc, unsigned char **pp);
ECC *d2i_ECCPrivateKey(ECC **ecc, unsigned char **pp, long length);

int i2d_ECCparams(ECC *ecc, unsigned char **pp);
ECC *d2i_ECCparams(ECC **ecc, unsigned char **pp, long length);

#ifdef  __cplusplus
}
#endif
#endif

