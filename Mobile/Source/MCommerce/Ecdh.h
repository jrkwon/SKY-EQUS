#ifndef HEADER_ECDH_H
#define HEADER_ECDH_H

#include "Ecc.h"

/*
 * * Made by Suengchur Pyun!!
 * * 
 * * e-mail : pyun@postech.edu
 * */

#ifdef  __cplusplus
extern "C" {
#endif

int ECDH_compute_key(unsigned char *key, Point *pub_key, ECC *ecc);

/* ANSI X9.63 Key Derivation Function */
int ANSI_X9_63_KDF(unsigned char *dst, unsigned long keylen,\
   				   unsigned char *secret, unsigned long size,\
   				   unsigned char *shared, unsigned long slen);

#ifdef  __cplusplus
}
#endif
#endif


