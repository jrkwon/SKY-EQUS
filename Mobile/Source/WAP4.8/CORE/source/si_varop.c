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
/*********
History
-------
990525 (KHN) Corrigendum WAG-WMLScript-29 implemented.
               Behaviour of division operator on an integer only device.
990623 (KHN) Corrected so that strings where one is a substring of the other
								are not considered equal. E.g. "Hello" and "Hello " are not
                considered equal any longer (function Var_EQ).

**********/
#include "si_var.h"

#include "si_misc.h"


/*
 * Increases the value with one
 * Modifes the variable!
 * Valid types: typeInteger, typeFloat
 * Returns TRUE if the type is compatible
 */
BOOL Var_INCR(pstructVar thisx)
{
	if ( thisx->type == typeInteger )
	{
		if ( thisx->val.theInt < INT32_MAX )
			thisx->val.theInt++;
		else
			Var_AssignInvalid(thisx);
		return( TRUE );
	}
#ifdef HAS_FLOAT
	else if ( thisx->type == typeFloat )
	{
		if ( thisx->val.theFloat < FLOAT32_MAX )
			thisx->val.theFloat += (FLOAT32)1;
		else
			Var_AssignInvalid(thisx);
		return( TRUE );
	}
#endif
	else
		return FALSE;
}


/* 
 * Decreases the value with one
 * Modifes the variable!
 * Valid types: typeInteger, typeFloat
 * Returns TRUE if the type is compatible
*/
BOOL Var_DECR(pstructVar thisx)
{
	if ( thisx->type == typeInteger )
	{
		if ( thisx->val.theInt > INT32_MIN )
			thisx->val.theInt--;
		else
			Var_AssignInvalid(thisx);
		return( TRUE );
	}
#ifdef HAS_FLOAT
	else if ( thisx->type == typeFloat )
	{
		if ( thisx->val.theFloat > -FLOAT32_MAX )
			thisx->val.theFloat -= (FLOAT32)1;
		else
			Var_AssignInvalid(thisx);
		return( TRUE );
	}
#endif
	else
		return FALSE;
}


/* 
 * Performs an unary minus on the value
 * Modifes the variable!
 * Valid types: typeInteger, typeFloat
 * Returns TRUE if the type is compatible
 */
pstructVar Var_UMINUS(const pstructVar a)
{
	pstructVar	res;

	if ( a->type == typeInteger )
	{
		res = Var_New();
		if (a->val.theInt != INT32_MIN)
    {
			Var_AssignInt(res, -a->val.theInt);
    }
    else
    {	/* (-) INT32_MIN causes overflow */
			Var_AssignInvalid(res);
    }
		return( res );
	}
#ifdef HAS_FLOAT
	else if ( a->type == typeFloat )
	{
		res = Var_New();
		Var_AssignFloat(res, -a->val.theFloat);
		return( res );
	}
#endif
	else
		return NULL;
}

static BOOL ADD_SanityCheck(INT32 a, INT32 b)
{
	INT32 c = a + b;

	if ( a > 0 && b > 0 )
		return( c > a );
	else if ( a < 0 && b < 0 )
		return( c < a );
	else
		return( TRUE );
}




/*
 * Performs an addition of the two values
 * Valid types: typeInteger, typeFloat, typeString
 * Returns a new Var if the types are compatible
 */
pstructVar Var_ADD(const pstructVar a, const pstructVar b)
{
	pstructVar	res = NULL;
	UINT32			newLen = 0;
	WCHAR				*newStr = NULL;
#ifdef HAS_FLOAT
	FLOAT32			fval = (FLOAT32)0;
#endif

	if ( a->type != b->type )
		return( NULL );

	if ( a->type == typeInteger )
	{
		res = Var_New();
		if ( ADD_SanityCheck(a->val.theInt, b->val.theInt) )
			Var_AssignInt(res, a->val.theInt + b->val.theInt);
		else
			Var_AssignInvalid(res);
		return( res );
	}
#ifdef HAS_FLOAT
	else if ( a->type == typeFloat )
	{
		res = Var_New();
		fval = a->val.theFloat + b->val.theFloat;
		Var_AssignFloat(res, fval);
		return( res );
	}
#endif
	else if ( a->type == typeString )
	{
		newLen = a->theStringLen + b->theStringLen;
		res = Var_New();
		if ( newLen > 0 )
		{
			newStr = NEWARRAY(WCHAR, newLen+1);
			newStr[newLen] = 0;	/* NULL terminated! */
			if ( a->val.theString != NULL )
				COPYSTRINGN(newStr, a->val.theString, a->theStringLen);
			if ( b->val.theString != NULL )
				COPYSTRINGN((newStr + (a->theStringLen)), b->val.theString, b->theStringLen);
			Var_AssignStringDirect(res, newLen, &newStr);
		}
		return( res );
	}
	else
		return NULL;
}


/*
 * Performs an subtraction of the two values
 * Valid types: typeInteger, typeFloat
 * Returns a new Var if the types are compatible
 */
pstructVar Var_SUB(const pstructVar a, const pstructVar b)
{
	pstructVar	res;
	FLOAT32			fval;

	if ( a->type != b->type )
		return( NULL );

	if ( a->type == typeInteger )
	{
		res = Var_New();
		if ( ADD_SanityCheck(a->val.theInt, -b->val.theInt) )
			Var_AssignInt(res, a->val.theInt - b->val.theInt);
		else
			Var_AssignInvalid(res);
		return( res );
	}
#ifdef HAS_FLOAT
	else if ( a->type == typeFloat )
	{
		res = Var_New();
		fval = a->val.theFloat - b->val.theFloat;
		Var_AssignFloat(res, fval);
		return( res );
	}
#endif
	else
		return NULL;
}


BOOL IntMUL_no_overflow(INT32 originala, INT32 originalb, INT32 *res)
{
	UINT32	a;
	UINT32	b;
	UINT32	a1;
	UINT32	a2;
	UINT32	b1;
	UINT32	b2;
  UINT32	middleRes1;
  UINT32	middleRes2;
  BOOL		fNegativeResult;
  UINT32	unsignedResult;

  /* First check for cases that won't work with the multiplication algoritm */

  if ((originala == 0) || (originalb == 0))
  {
		*res = 0;
  	return TRUE;
  }
  if ((originala == INT32_MIN) || (originalb == INT32_MIN))
  {
  	if ((originala == 1) || (originalb == 1))
    {
			*res = INT32_MIN;
    	return TRUE;
    }
    else
    {
    	return FALSE;
    }
  }

  /* now init all variables */

 	/* res without sign = a * b = (a1<<16 + a2)*(b1<<16 + b2) =
	 										= (a1 * b1)<<32 + (a1*b2 + a2*b1)<<16 + (a2*b2)  */

	fNegativeResult = ((originala < 0) ^ (originalb < 0));
	a = (UINT32) ABS(originala);
	b = (UINT32) ABS(originalb);
	a1 = (UINT16)( (a>>16) & 0x0000FFFF );
	a2 = (UINT16)( a & 0x0000FFFF );
	b1 = (UINT16)( (b>>16) & 0x0000FFFF );
	b2 = (UINT16)( b & 0x0000FFFF );

  /* now perform the multiplication while watching for overflow */

	if ((a1 == 0) || (b1 == 0))
  {
  	middleRes1 = a1*b2 + a2*b1;
    if (middleRes1 <= 0x0000FFFF)
    {
    	middleRes1 <<= 16;
      middleRes2 = a2*b2;
      unsignedResult = middleRes1 + middleRes2;

      if ((unsignedResult & 0x80000000) == 0)
      {	/* check so that the 32nd bit is not 1 (then it would be overflow) */
       	*res = *(INT32*)((VOID*)&unsignedResult);

				if (fNegativeResult)
        {
        	*res = - (*res);
        }
      	return TRUE;
      }
      else if (fNegativeResult && (unsignedResult == 0x80000000))
      {	/* INT32_MIN is a valid result */
      	*res = INT32_MIN;
        return TRUE;
      }
    }
  }
	/* overflow if this line is reached */
	return FALSE;
}


/*
 * Performs an multiplication of the two values
 * Valid types: typeInteger, typeFloat
 * Returns a new Var if the types are compatible
 */
pstructVar Var_MUL(const pstructVar a, const pstructVar b)
{
	pstructVar	res;
	INT32				ival;
	FLOAT32			fval;

	if ( a->type != b->type )
		return( NULL );

	if ( a->type == typeInteger )
	{
		res = Var_New();
		if ( IntMUL_no_overflow(a->val.theInt, b->val.theInt, &ival) )
			Var_AssignInt(res, ival);
		else
			Var_AssignInvalid(res);
		return( res );
	}
#ifdef HAS_FLOAT
	else if ( a->type == typeFloat )
  {
		res = Var_New();
		fval = a->val.theFloat * b->val.theFloat;
		Var_AssignFloat(res, fval);
		return( res );
	}
#endif
	else
		return NULL;
}


/*
 * Performs an division of the two values
 * Valid types: typeInteger, typeFloat
 * Returns a new Var if the types are compatible
 */
pstructVar Var_DIV(const pstructVar a, const pstructVar b)
{
	pstructVar	res;
	FLOAT32			fval;

	if ( a->type != b->type )
		return( NULL );

	if ( a->type == typeInteger )
	{
		res = Var_New();
		if ( b->val.theInt == 0 )
			Var_AssignInvalid(res);
		else
#ifdef HAS_FLOAT
			Var_AssignFloat(res, (FLOAT32)a->val.theInt / b->val.theInt);
#else
			Var_AssignInvalid(res);
#endif
		return( res );
	}
#ifdef HAS_FLOAT
	else if ( a->type == typeFloat )
	{
		res = Var_New();
		if ( FloatIsZero(b->val.theFloat))
    {
			Var_AssignInvalid(res);
    }
		else
		{
			fval = a->val.theFloat / b->val.theFloat;
			Var_AssignFloat(res, fval);
    }
		return( res );
	}
#endif
	else
		return NULL;
}


/* 
 * Performs an integer division of the two values
 * Valid types: typeInteger
 * Returns a new Var if the types are compatible
 */
pstructVar Var_IDIV(const pstructVar a, const pstructVar b)
{
	pstructVar	res;

	if ( a->type != b->type || a->type != typeInteger )
		return( NULL );

	res = Var_New();
	if ( b->val.theInt == 0 )
		Var_AssignInvalid(res);
	else
		Var_AssignInt(res, a->val.theInt / b->val.theInt);
	return( res );
}


/* 
 * Performs an reminder operation of the two values
 * Valid types: typeInteger
 * Returns a new Var if the types are compatible
 */
pstructVar Var_REM(const pstructVar a, const pstructVar b)
{
	pstructVar	res;

	if ( a->type != b->type || a->type != typeInteger )
		return( NULL );

	res = Var_New();
	if ( b->val.theInt == 0 )
		Var_AssignInvalid(res);
	else
		Var_AssignInt(res, a->val.theInt % b->val.theInt);
	return( res );
}



/* 
 * Performs an bitwise and of the two values
 * Valid types: typeInteger
 * Returns a new Var if the types are compatible
 */
pstructVar Var_B_AND(const pstructVar a, const pstructVar b)
{
	pstructVar	res;

	if ( a->type != b->type || a->type != typeInteger )
		return( NULL );

	res = Var_New();
	Var_AssignInt(res, a->val.theInt & b->val.theInt);
	return( res );
}


/* 
 * Performs an bitwise or of the two values
 * Valid types: typeInteger
 * Returns a new Var if the types are compatible
 */
pstructVar Var_B_OR(const pstructVar a, const pstructVar b)
{
	pstructVar	res;

	if ( a->type != b->type || a->type != typeInteger )
		return( NULL );

	res = Var_New();
	Var_AssignInt(res, a->val.theInt | b->val.theInt);
	return( res );
}


/* 
 * Performs an bitwise xor of the two values
 * Valid types: typeInteger
 * Returns a new Var if the types are compatible
 */
pstructVar Var_B_XOR(const pstructVar a, const pstructVar b)
{
	pstructVar	res;

	if ( a->type != b->type || a->type != typeInteger )
		return( NULL );

	res = Var_New();
	Var_AssignInt(res, a->val.theInt ^ b->val.theInt);
	return( res );
}


/* 
 * Performs an bitwise not on the operand
 * Valid types: typeInteger
 * Returns a new Var if the type are compatible
 */
pstructVar Var_B_NOT(const pstructVar a)
{
	pstructVar	res;

	if ( a->type != typeInteger )
		return( NULL );

	res = Var_New();
	Var_AssignInt(res, ~a->val.theInt);
	return( res );
}


/* 
 * Performs an bitwise left shift on the operand
 * Valid types: typeInteger
 * Returns a new Var if the types are compatible
 */
pstructVar Var_B_LSHIFT(const pstructVar a, const pstructVar b)
{
	pstructVar	res;

	if ( a->type != b->type || a->type != typeInteger )
		return( NULL );

  if ( b->val.theInt < 0 )
  {
		res = Var_New();
		Var_AssignInvalid(res);
  	return res;
  }

	res = Var_New();
	if (b->val.theInt < 32)
  {
		Var_AssignInt(res, a->val.theInt << b->val.theInt);
  }
  else
  {
		Var_AssignInt(res, 0 );
  }
	return( res );
}


/*
 * Performs an bitwise signed right shift on the operand
 * Valid types: typeInteger
 * Returns a new Var if the types are compatible
 */
pstructVar Var_B_RSSHIFT(const pstructVar a, const pstructVar b)
{
	pstructVar	res;

	if ( a->type != b->type || a->type != typeInteger )
		return( NULL );

  if ( b->val.theInt < 0 )
  {
		res = Var_New();
		Var_AssignInvalid(res);
  	return res;
  }

	if ( a->val.theInt < 0 )
	{	/* new bits must be the same as the sign bit (i.e. ones if it is a negative number) */
		res = Var_New();
		if (b->val.theInt < 32)
	  {	/* not shifting more than 32 bits */
			Var_AssignInt(res, (a->val.theInt >> b->val.theInt) | ( ~((INT32)0) << (32 - b->val.theInt) ) );
	  }
	  else
	  {	/* shifting more than 32 bits right with operation results in 32 ones */
			Var_AssignInt(res, ~((INT32)0) );
	  }
		return( res );
	}
  else
  {	/* for positive integers, RSSHIFT works the same as RSZSHIFT */
		return Var_B_RSZSHIFT(a,b);
  }
}


/*
 * Performs an bitwise right shift on the operand
 * Valid types: typeInteger
 * Returns a new Var if the types are compatible
 */
pstructVar Var_B_RSZSHIFT(const pstructVar a, const pstructVar b)
{
	pstructVar	res;

	if ( a->type != b->type || a->type != typeInteger )
		return( NULL );

  if ( b->val.theInt < 0 )
  {
		res = Var_New();
		Var_AssignInvalid(res);
  	return res;
  }

	res = Var_New();
	if (b->val.theInt < 32)
  {	/* not shifting more than 32 bits */
		Var_AssignInt(res, (INT32)(((UINT32)a->val.theInt) >> b->val.theInt) );
    	/* The ANSI C standard says that right shift with unsigned ints shall be filled with zeroes
      therefore the cast to UINT32 */
  }
  else
  {	/* shifting more than 32 bits right with zero operation results in 0 */
		Var_AssignInt(res, 0 );
  }
	return( res );
}


/*
 * Performs an equality comparison
 * Valid types: typeInteger, typeFloat, typeString
 * Returns a new Var if the types are compatible
 */
pstructVar Var_EQ(const pstructVar a, const pstructVar b)
{
	pstructVar	res;

	if ( a->type != b->type )
		return( NULL );

	if ( a->type == typeInteger )
	{
		res = Var_New();
		Var_AssignBool(res, (BOOL)(a->val.theInt == b->val.theInt));
		return( res );
	}
#ifdef HAS_FLOAT
	else if ( a->type == typeFloat )
	{
/*
 * The following code is the "close enough" equality check.
 *
		FLOAT32 a_minus_b_abs = (a->val.theFloat > b->val.theFloat)?
    		(a->val.theFloat - b->val.theFloat):(b->val.theFloat - a->val.theFloat);
  	FLOAT32 a_abs = (a->val.theFloat<0)?(-a->val.theFloat):(a->val.theFloat);

		res = Var_New();
		Var_AssignBool(res, (BOOL)(a_minus_b_abs < (FLOAT32_EPSILON * a_abs)));
		return( res );
*/
		res = Var_New();
		if (FloatIsZero(a->val.theFloat) && FloatIsZero(b->val.theFloat))
    {/* to make sure that -0.0 == +0.0 */
			Var_AssignBool(res, TRUE);
    }
    else
    {
			Var_AssignBool(res, (BOOL)(a->val.theFloat == b->val.theFloat));
    }
		return( res );
	}
#endif
	else if ( a->type == typeString )
	{
		res = Var_New();
		if ((a->val.theString != NULL) && (b->val.theString != NULL) &&
    		( a->theStringLen == b->theStringLen ))
		{
			Var_AssignBool(res, (BOOL)(0==COMPARESTRINGN(a->val.theString, b->val.theString, a->theStringLen)));
			return( res );
		}
		else if ((a->val.theString == NULL) && (b->val.theString == NULL))
		{
			Var_AssignBool(res, TRUE);
			return( res );
		}
		else
		{
			Var_AssignBool(res, FALSE);
			return( res );
		}
	}
	else
		return NULL;
}


/* 
 * Performs an less than or equal comparison
 * Valid types: typeInteger, typeFloat, typeString
 * Returns a new Var if the types are compatible
 */
pstructVar Var_LE(const pstructVar a, const pstructVar b)
{
	pstructVar	res;

	if ( a->type != b->type )
		return( NULL );

	if ( a->type == typeInteger )
	{
		res = Var_New();
		Var_AssignBool(res, (BOOL)(a->val.theInt <= b->val.theInt));
		return( res );
	}
#ifdef HAS_FLOAT
	else if ( a->type == typeFloat )
		{
		res = Var_New();
		Var_AssignBool(res, (BOOL)(a->val.theFloat <= b->val.theFloat));
		return( res );
		}
#endif
	else if ( a->type == typeString )
	{
		res = Var_New();
		if ((a->val.theString != NULL) && (b->val.theString != NULL))
		{
			Var_AssignBool(res, (BOOL)(0>=COMPARESTRINGN(a->val.theString, b->val.theString, a->theStringLen)));
			return( res );
		}
		else if (((a->val.theString == NULL) && (b->val.theString == NULL)) ||
							((a->val.theString == NULL) && (b->val.theString != NULL)))
		{
			Var_AssignBool(res, TRUE);
			return( res );
		}
		else
		{
			Var_AssignBool(res, FALSE);
			return( res );
		}
	}
	else
		return NULL;
}


/* 
 * Performs an less than comparison
 * Valid types: typeInteger, typeFloat, typeString
 * Returns a new Var if the types are compatible
 */
pstructVar Var_LT(const pstructVar a, const pstructVar b)
{
	pstructVar	res;

	if ( a->type != b->type )
		return( NULL );

	if ( a->type == typeInteger )
	{
		res = Var_New();
		Var_AssignBool(res, (BOOL)(a->val.theInt < b->val.theInt));
		return( res );
	}
#ifdef HAS_FLOAT
	else if ( a->type == typeFloat )
		{
		res = Var_New();
		Var_AssignBool(res, (BOOL)(a->val.theFloat < b->val.theFloat));
		return( res );
		}
#endif
	else if ( a->type == typeString )
	{
		res = Var_New();
		if ((a->val.theString != NULL) && (b->val.theString != NULL))
		{
			Var_AssignBool(res, (BOOL)(0>COMPARESTRINGN(a->val.theString, b->val.theString, a->theStringLen)));
			return( res );
		}
		else if ((a->val.theString == NULL) && (b->val.theString != NULL))
		{
			Var_AssignBool(res, TRUE);
			return( res );
		}
		else
		{
			Var_AssignBool(res, FALSE);
			return( res );
		}
	}
	else
		return NULL;
}


/* 
 * Performs an greater than or equal comparison
 * Valid types: typeInteger, typeFloat, typeString
 * Returns a new Var if the types are compatible
 */
pstructVar Var_GE(const pstructVar a, const pstructVar b)
{
	pstructVar	res;

	if ( a->type != b->type )
		return( NULL );

	if ( a->type == typeInteger )
	{
		res = Var_New();
		Var_AssignBool(res, (BOOL)(a->val.theInt >= b->val.theInt));
		return( res );
	}
#ifdef HAS_FLOAT
	else if ( a->type == typeFloat )
		{
		res = Var_New();
		Var_AssignBool(res, (BOOL)(a->val.theFloat >= b->val.theFloat));
		return( res );
		}
#endif
	else if ( a->type == typeString )
	{
		res = Var_New();
		if ((a->val.theString != NULL) && (b->val.theString != NULL))
		{
			Var_AssignBool(res, (BOOL)(0<=COMPARESTRINGN(a->val.theString, b->val.theString, a->theStringLen)));
			return( res );
		}
		else if (((a->val.theString == NULL) && (b->val.theString == NULL)) ||
							((a->val.theString != NULL) && (b->val.theString == NULL)))
		{
			Var_AssignBool(res, TRUE);
			return( res );
		}
		else
		{
			Var_AssignBool(res, FALSE);
			return( res );
		}
	}
	else
		return NULL;
}


/* 
 * Performs an greater than comparison
 * Valid types: typeInteger, typeFloat, typeString
 * Returns a new Var if the types are compatible
 */
pstructVar Var_GT(const pstructVar a, const pstructVar b)
{
	pstructVar	res;

	if ( a->type != b->type )
		return( NULL );

	if ( a->type == typeInteger )
	{
		res = Var_New();
		Var_AssignBool(res, (BOOL)(a->val.theInt > b->val.theInt));
		return( res );
	}
#ifdef HAS_FLOAT
	else if ( a->type == typeFloat )
		{
		res = Var_New();
		Var_AssignBool(res, (BOOL)(a->val.theFloat > b->val.theFloat));
		return( res );
		}
#endif
	else if ( a->type == typeString )
	{
		res = Var_New();
		if ((a->val.theString != NULL) && (b->val.theString != NULL))
		{
			Var_AssignBool(res, (BOOL)(0<COMPARESTRINGN(a->val.theString, b->val.theString, a->theStringLen)));
			return( res );
		}
		else if ((a->val.theString != NULL) && (b->val.theString == NULL))
		{
			Var_AssignBool(res, TRUE);
			return( res );
		}
		else
		{
			Var_AssignBool(res, FALSE);
			return( res );
		}
	}
	else
		return NULL;
}


