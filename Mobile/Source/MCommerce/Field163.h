#ifndef HEADER_FIELD_H
#define HEADER_FIELD_H

//swjung
#ifdef BLOCK
	#include <bn.h>
#endif //#ifdef BLOCK

#include "bn_new.h"

/*
 * Made by Suengchur Pyun!!
 * 
 * e-mail : pyun@postech.edu
 */

#ifdef  __cplusplus
extern "C" {
#endif

typedef unsigned short	WORD16;
typedef unsigned long	WORD32;

int Init_SqrTab(void);

int Add163(WORD32 *C, WORD32 *A, WORD32 *B);
int Sqr163(WORD32 *C, WORD32 *A);
int Mul163(WORD32 *C, WORD32 *A, WORD32 *B);
int Inv163(WORD32 *C, WORD32 *A);

int IsZero163(WORD32 *A);
int IsEqual163(WORD32 *A, WORD32 *B);
int SetZero163(WORD32 *A);
int Copy163(WORD32 *B, WORD32 *A);
int Print163(const char *str, WORD32 *A);

int Field2Bn(BIGNUM *B, WORD32 *A);
int Bn2Field(WORD32 *B, BIGNUM *A, int size);
int Field2Octet(unsigned char *to, WORD32 *A, int fsize);
int Octet2Field(WORD32 *A, unsigned char *B, int octlen);


#ifdef  __cplusplus
}
#endif
#endif

