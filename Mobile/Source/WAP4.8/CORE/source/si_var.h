/*
 * Copyright (C) Ericsson Mobile Communications AB, 2000.
 * Licensed to AU-System AB.
 * All rights reserved.
 *
 * This software is covered by the license agreement between
 * the end user and AU-System AB, and may be used and copied
 * only in accordance with the terms of the said agreement.
 *
 * Neither Ericsson Mobile Communications AB nor AU-System AB
 * assumes any responsibility or liability for any errors or inaccuracies in
 * this software, or any consequential, incidental or indirect damage arising
 * out of the use of the Generic WAP Client software.
 */
#ifndef _SI_VAR_H
#define _SI_VAR_H

#include "cmmnrsrc.h"

typedef enum enumVarType {
	typeInteger = 0,
	typeFloat = 1,
	typeString = 2,
	typeBool = 3,
	typeInvalid = 4
} enumVarType;


typedef struct
{
	enumVarType				type;
	union {
		INT32						theInt;
#ifdef HAS_FLOAT
		FLOAT32					theFloat;
#endif
		WCHAR*						theString;
	} val;
	UINT32						theStringLen;
} structVar, *pstructVar, **ppstructVar;


typedef enum { 
	CONVERT_BOOLS,
	CONVERT_INTS,
	CONVERT_FLOATS,
	CONVERT_STRINGS,
	CONVERT_INT_FLOAT,
	CONVERT_INTS_FLOATS,
	CONVERT_INTS_FLOATS_STRINGS
} enumConvertMethod;

typedef enum {
	VCR_OK,       /* can convert */
	VCR_NOT_OK,   /* can not convert */
	VCR_OVERFLOW  /* content has ok syntax for conversion but incorrect value
                       Example: a string containing an integer too large to be saved
                       in a 32 bit integer */
} enumVarConvertResult;


pstructVar Var_New( VOID );
VOID Var_Delete( pstructVar* pThis );

BOOL Var_ConvertMethod( enumConvertMethod method, pstructVar op1, pstructVar op2 );
enumVarConvertResult Var_Convert(pstructVar thisx, enumVarType toType );
VOID Var_AssignVar(pstructVar thisx, pstructVar src );
VOID Var_AssignInt(pstructVar thisx, INT32 val);
VOID Var_AssignString(pstructVar thisx, UINT32 len, WCHAR* str); /* by copying */
VOID Var_AssignStringDirect(pstructVar thisx, UINT32 len, WCHAR** str); /* NO copying */
VOID Var_NewString(pstructVar thisx, UINT32 len);
VOID Var_AssignBool(pstructVar thisx, BOOL val);
VOID Var_AssignInvalid(pstructVar thisx);
#ifdef HAS_FLOAT
VOID Var_AssignFloat(pstructVar thisx, FLOAT32 val);
#endif

/******************************************************************/
/* The following arithmetic functions requires that both operands */
/* are of the same and correct types                              */
/******************************************************************/

/* The following two functions modifies the arguments */
BOOL Var_INCR(pstructVar thisx);
BOOL Var_DECR(pstructVar thisx);

/* The following functions doesn't modify the arguments */
/* They return NULL if the arguments isn't correct      */
pstructVar Var_UMINUS(const pstructVar a);
pstructVar Var_ADD(const pstructVar a, const pstructVar b);
pstructVar Var_SUB(const pstructVar a, const pstructVar b);
pstructVar Var_MUL(const pstructVar a, const pstructVar b);
pstructVar Var_DIV(const pstructVar a, const pstructVar b);
pstructVar Var_IDIV(const pstructVar a, const pstructVar b);
pstructVar Var_REM(const pstructVar a, const pstructVar b);
pstructVar Var_B_AND(const pstructVar a, const pstructVar b);
pstructVar Var_B_OR(const pstructVar a, const pstructVar b);
pstructVar Var_B_XOR(const pstructVar a, const pstructVar b);
pstructVar Var_B_NOT(const pstructVar a);
pstructVar Var_B_LSHIFT(const pstructVar a, const pstructVar b);
pstructVar Var_B_RSSHIFT(const pstructVar a, const pstructVar b);
pstructVar Var_B_RSZSHIFT(const pstructVar a, const pstructVar b);
pstructVar Var_EQ(const pstructVar a, const pstructVar b);
pstructVar Var_LE(const pstructVar a, const pstructVar b);
pstructVar Var_LT(const pstructVar a, const pstructVar b);
pstructVar Var_GE(const pstructVar a, const pstructVar b);
pstructVar Var_GT(const pstructVar a, const pstructVar b);


BOOL IntMUL_no_overflow(INT32 originala, INT32 originalb, INT32 *res);


/* _SI_VAR_H */
#endif
