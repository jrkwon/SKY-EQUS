#ifndef HEADER_GROUP_H
#define HEADER_GROUP_H

#include "Field163.h"
#include <stdlib.h>
#include <string.h>
#include "crypto.h"

/*
 * Made by Suengchur Pyun!!
 * 
 * e-mail : pyun@postech.edu
 */

#undef NO_INIFNITE
//#define NO_INIFINTE
#ifdef  __cplusplus
extern "C" {
#endif

typedef struct _point
{
	WORD32 x[6], y[6];
}Point;

typedef struct _curve
{
	WORD32 a[6], b[6];
}Curve;

Point *Point_new(void);
void Point_free(Point *);
Curve *Curve_new(void);
void Curve_free(Curve *);

int Eadd(Point *C, Point *A, Point *B, Curve *Cr);
int Esub(Point *C, Point *A, Point *B, Curve *Cr);
int Edbl(Point *C, Point *A, Curve *Cr);

int Init_PTab(Point *A, Curve *Cr);
int PMul(Point *C, Point *A, BIGNUM *k, BIGNUM *n, Curve *Cr);
int BrickellMul(Point *R, Point *P, BIGNUM *k, BIGNUM *n, Curve *Cr);

int ValidPoint(Point *A, Curve *Cr);
int MakePoint(Point *C, Curve *Cr);

int OctetSizeOfPoint(int form);
unsigned char *Point2Octet(Point *A, int form);
int Octet2Point(Point *A, unsigned char *B, int llen);

#ifdef  __cplusplus
}
#endif
#endif

