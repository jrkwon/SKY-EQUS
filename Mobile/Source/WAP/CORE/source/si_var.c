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
#include "si_var.h"

#ifdef HAS_FLOAT
	#include "si_float.h"
#endif

#include "si_int.h"
#include "si_misc.h"



static VOID Var_Empty(pstructVar thisx)
{
	if ( thisx->type == typeString && thisx->val.theString != NULL )
	{
		DEALLOC(&(thisx->val.theString));
	}
}


pstructVar Var_New( VOID )
{
	pstructVar var;

	var = NEWSTRUCT(structVar);
	var->type = typeString;
	var->theStringLen = 0;
	var->val.theString = NULL;
	return var;
}


VOID Var_Delete(pstructVar* pThis)
{
	if ( *pThis == NULL )
		return;

	Var_Empty(*pThis);
	DEALLOC(pThis);
}


BOOL Var_ConvertMethod( enumConvertMethod method, pstructVar op1, pstructVar op2 )
{
	BOOL res = TRUE;
	enumVarConvertResult convRes;
	pstructVar	a, b;

	switch ( method )
	{

		case CONVERT_BOOLS:
			if ( op1 == NULL || op1->type == typeInvalid )
			{
				res = FALSE;
				break;
			}
			res = (VCR_OK == Var_Convert(op1, typeBool));
			if ( op2 && res == TRUE )
			{
      	if (op2->type != typeInvalid)
        {
					res = (VCR_OK == Var_Convert(op2, typeBool));
        }
        else
        {
        	res = FALSE;
        }
			}
			break;


		case CONVERT_INTS:
			if ( op1 == NULL || op1->type == typeInvalid )
			{
				res = FALSE;
				break;
			}
			res = (VCR_OK == Var_Convert(op1, typeInteger));
			if ( op2 && res == TRUE )
			{
      	if (op2->type != typeInvalid)
        {
					res = (VCR_OK == Var_Convert(op2, typeInteger));
        }
        else
        {
        	res = FALSE;
        }
			}
			break;


		case CONVERT_FLOATS:
#ifdef HAS_FLOAT
			if ( op1 == NULL || op1->type == typeInvalid )
			{
				res = FALSE;
				break;
			}
			res = (VCR_OK == Var_Convert(op1, typeFloat));
			if ( op2 && res == TRUE )
			{
      	if (op2->type != typeInvalid)
        {
					res = (VCR_OK == Var_Convert(op2, typeFloat));
        }
        else
        {
        	res = FALSE;
        }
			}
#else
			res = FALSE;
#endif
			break;


		case CONVERT_STRINGS:
			if ( op1 == NULL || op1->type == typeInvalid )
			{
				res = FALSE;
				break;
			}
			res = (VCR_OK == Var_Convert(op1, typeString));
			if ( op2 && res == TRUE )
			{
      	if (op2->type != typeInvalid)
        {
					res = (VCR_OK == Var_Convert(op2, typeString));
        }
        else
        {
        	res = FALSE;
        }
			}
			break;


		case CONVERT_INT_FLOAT: /* Unary */
			if ( op1 == NULL || op2 != NULL || op1->type == typeInvalid )
			{
				res = FALSE;
				break;
			}
			convRes = Var_Convert(op1, typeInteger);
      if (convRes == VCR_OVERFLOW)
      {
      	/* conversion failed due to overflow and there will be no try
        to convert it to a float */
        res = FALSE;
        break;
      }
      res = (convRes == VCR_OK);
#ifdef HAS_FLOAT
			if ( res == FALSE )
			{
				res = (VCR_OK == Var_Convert(op1, typeFloat));
			}
#endif
			break;


		case CONVERT_INTS_FLOATS:
			if ( op1 == NULL || op2 == NULL || op1->type == typeInvalid || op2->type == typeInvalid)
			{
				res = FALSE;
				break;
			}

#ifdef HAS_FLOAT
			if ( op1->type == typeFloat || op2->type == typeFloat )
			{
				res = (VCR_OK == Var_Convert(op1, typeFloat));
				if ( res == TRUE )
				{
					res = (VCR_OK == Var_Convert(op2, typeFloat));
				}
				break;
			}

			a = Var_New();
			b = Var_New();

			Var_AssignVar(a, op1);
			Var_AssignVar(b, op2);

			convRes = Var_Convert(a, typeInteger);
			if ( convRes == VCR_OK )
			{
				convRes = Var_Convert(b, typeInteger);
			}

      if (convRes == VCR_OVERFLOW)
      {
      	/* conversion failed due to overflow and there will be no try
        to convert to float */
        res = FALSE;
				Var_Delete(&a);
				Var_Delete(&b);
        break;
      }
      res = (convRes == VCR_OK);

			if ( res == TRUE )
			{
				Var_AssignVar(op1, a);
				Var_AssignVar(op2, b);
			}
			else
			{
				Var_AssignVar(a, op1);
				Var_AssignVar(b, op2);

				res = (VCR_OK == Var_Convert(a, typeFloat));
				if ( res == TRUE )
				{
					res = (VCR_OK == Var_Convert(b, typeFloat));
				}
				if ( res == TRUE )
				{
					Var_AssignVar(op1, a);
					Var_AssignVar(op2, b);
				}
			}

			Var_Delete(&a);
			Var_Delete(&b);
#else
			res = (VCR_OK == Var_Convert(op1, typeInteger));
			if ( res == TRUE )
			{
				res = (VCR_OK == Var_Convert(op2, typeInteger));
			}
#endif
			break;


		case CONVERT_INTS_FLOATS_STRINGS:
			res = FALSE;
			if ( op1 == NULL || op2 == NULL || op1->type == typeInvalid || op2->type == typeInvalid)
			{
				break;
			}
			
			if ( op1->type == typeString || op2->type == typeString )
			{
				res = (VCR_OK == Var_Convert(op1, typeString));
				if ( res == TRUE )
				{
					res = (VCR_OK == Var_Convert(op2, typeString));
				}
				break;
			}
			
#ifdef HAS_FLOAT
			if ( op1->type == typeFloat || op2->type == typeFloat )
			{
				res = (VCR_OK == Var_Convert(op1, typeFloat));
				if ( res == TRUE )
				{
					res = (VCR_OK == Var_Convert(op2, typeFloat));
				}
				break;
			}
#endif

			res = (VCR_OK == Var_Convert(op1, typeInteger));
			if ( res == TRUE )
			{
				res = (VCR_OK == Var_Convert(op2, typeInteger));
			}
			break;
	}

	return( res );
}


/* Tries to convert the variable "thisx" to type "toType".              */
/* Returns VCR_OK if the conversion was succesfull.                      */
/* VCR_NOT_OK is returned if the variable couldn't be converted, and        */
/* in that case the variable "thisx" isn't modified.                    */
/* VCR_OVERFLOW is returned if a string to integer convertion overflows */
enumVarConvertResult Var_Convert(pstructVar thisx, enumVarType toType )
{
	enumVarConvertResult		res = VCR_OK;
  BOOL    boolRes = TRUE;
	WCHAR		*str = NULL;
	UINT32	len = 0;

	WCHAR		wc_true[]  = {'t','r','u','e', 0};
	WCHAR		wc_false[] = {'f','a','l','s','e', 0};


	if ( thisx->type == typeInvalid )
		return VCR_NOT_OK;

	switch ( toType )
	{
		case typeInteger:
			switch ( thisx->type )
			{
				case typeFloat:
					/* Not allowed */
					res = VCR_NOT_OK;
					break;
				case typeString:
					{
					UINT8	charsUsed;
					BOOL	isOverflow;
					INT32	tmpInt;

					/* Is there something to convert? */
					if ( thisx->val.theString == NULL || thisx->theStringLen == 0 )
						{
						res = VCR_NOT_OK;
						break;
						}

					/* OK, try to convert the string */
					thisx->val.theString[thisx->theStringLen] = 0;
					boolRes = String2Int(thisx->val.theString, &charsUsed, &tmpInt, &isOverflow);

					/* Exit if it wasn't a valid number in the string */
					if ( boolRes == FALSE )
          {
						if (isOverflow)
            {
							res = VCR_OVERFLOW;
            }
            else
            {
							res = VCR_NOT_OK;
            }
						break;
          }

					/* Is all characters used or is there only whitespace left in the string? */
					if ( charsUsed == thisx->theStringLen )
						{
						/* All is OK! */
						Var_AssignInt(thisx, tmpInt);
						break;
						}
					str = thisx->val.theString + charsUsed;
					len = thisx->theStringLen - charsUsed;
					while ( len )
						{
						if ( ! IsWhitespaceChar(*str) )
							{
							res = VCR_NOT_OK;
							break;
							}
						len--;
						}

					if (res == VCR_OK)
						{
						/* All is OK! */
						Var_AssignInt(thisx, tmpInt);
						}
					}
					break;
			}
			break;

		case typeFloat:
#ifdef HAS_FLOAT
			switch ( thisx->type )
			{
				case typeBool:
				case typeInteger:
					thisx->val.theFloat = (FLOAT32)thisx->val.theInt;
					break;
				case typeString:
					{
					UINT8	  charsUsed;
					BOOL	  isOverflow;
          FLOAT32 f;

					/* Is there something to convert? */
					if ( thisx->val.theString == NULL || thisx->theStringLen == 0 )
						{
						res = VCR_NOT_OK;
						break;
						}

					/* OK, try to convert the string */
					thisx->val.theString[thisx->theStringLen] = 0;
					boolRes = String2Float (thisx->val.theString, &charsUsed,
                                  &f, &isOverflow);

					/* Exit if it wasn't a valid number in the string */
					if ( boolRes == FALSE )
          {
						res = VCR_NOT_OK;
						break;
          }

					/* Is all characters used or is there only whitespace left in the string? */
					if ( charsUsed == thisx->theStringLen )
						{
						/* All is OK! */
						Var_AssignFloat(thisx, f);
						break;
						}
					str = thisx->val.theString + charsUsed;
					len = thisx->theStringLen - charsUsed;
					while ( len )
						{
						if ( ! IsWhitespaceChar(*str) )
							{
							res = VCR_NOT_OK;
							break;
							}
						len--;
						}

					if (res == VCR_OK) 
						{
						/* All is OK! */
						Var_AssignFloat (thisx, f);
						}
					}
					break;
			}
#else 
			res = VCR_NOT_OK;
#endif
			break;

		case typeString:
			switch ( thisx->type )
			{
				case typeInteger:
					{
					WCHAR tmp[20];

					Int2String(thisx->val.theInt, tmp);
					Var_AssignString(thisx, STRINGLENGTH(tmp), tmp);
					}
					break;
				case typeFloat:
#ifdef HAS_FLOAT
					{
					WCHAR			tmp[50];

					Float2String (thisx->val.theFloat, tmp);
					Var_AssignString(thisx, STRINGLENGTH(tmp), tmp);
					}
#else
					res = VCR_NOT_OK;
#endif
					break;
				case typeBool:
					Var_Empty(thisx);
					if ( thisx->val.theInt == 0 )
						Var_AssignString(thisx, 5, wc_false);
					else
						Var_AssignString(thisx, 4, wc_true);
					break;
			}
			break;

		case typeBool:
			switch ( thisx->type )
			{
				case typeInteger:
					thisx->val.theInt = ((thisx->val.theInt == 0) ? 0 : 1);
					break;
				case typeFloat:
#ifdef HAS_FLOAT
					thisx->val.theInt = ((thisx->val.theFloat == 0.0) ? 0 : 1);
#else
					res = VCR_NOT_OK;
#endif
					break;
				case typeString:
					Var_AssignBool( thisx, ((thisx->theStringLen > 0) ? TRUE : FALSE));
					break;
			}
			break;

		case typeInvalid:
			Var_Empty(thisx);
			break;
	}

	if ( res == VCR_OK )
		thisx->type = toType;
	return( res );
}


VOID Var_AssignVar(pstructVar thisx, pstructVar src )
{
	Var_Empty(thisx);

	thisx->type = src->type;
	switch ( thisx->type )
	{
		case typeString:
			thisx->theStringLen = src->theStringLen;
			if ( thisx->theStringLen > 0 )
			{
				thisx->val.theString = NEWARRAY(WCHAR, thisx->theStringLen+1);
				thisx->val.theString[thisx->theStringLen] = 0;
				COPYSTRINGN(thisx->val.theString, src->val.theString, thisx->theStringLen);
			}
			else
				thisx->val.theString = NULL;
			break;
		case typeInteger:
		case typeBool:
			thisx->val.theInt = src->val.theInt;
			break;
		case typeFloat:
#ifdef HAS_FLOAT
			thisx->val.theFloat = src->val.theFloat;
#else
			thisx->type = typeInvalid;
#endif
			break;
		case typeInvalid:
			thisx->val.theInt = 0;
			break;
	}
}


VOID Var_AssignInt(pstructVar thisx, INT32 val)
{
	Var_Empty(thisx);
	thisx->type = typeInteger;
	thisx->val.theInt = val;
}


VOID Var_AssignFloat(pstructVar thisx, FLOAT32 val)
{
	Var_Empty(thisx);
#ifdef HAS_FLOAT
	thisx->type = typeFloat;

	if ( ! IsLegalFloatVal(val) )
  {
		thisx->type = typeInvalid;
	}
	else if ( IsUnderflow(val) )
  {
		thisx->val.theFloat = (FLOAT32)0;
  }
  else
  {
		thisx->val.theFloat = val;
  }
#else
	thisx->type = typeInvalid;
#endif
}


VOID Var_AssignString(pstructVar thisx, UINT32 len, WCHAR* str)
{
	Var_Empty(thisx);
	thisx->type = typeString;
	thisx->theStringLen = len;
	thisx->val.theString = NEWARRAY(WCHAR, len+1);
	COPYSTRINGN(thisx->val.theString, str, len);
	thisx->val.theString[len] = 0;
}


VOID Var_AssignStringDirect(pstructVar thisx, UINT32 len, WCHAR** str)
/* the string is taken as is, not copied! */
{
	Var_Empty(thisx);
	thisx->type = typeString;
	thisx->theStringLen = len;
	thisx->val.theString = *str;
	*str = NULL;
}


VOID Var_NewString(pstructVar thisx, UINT32 len)
{
	if (len > 0)
		{
		Var_Empty(thisx);
		thisx->type = typeString;
		thisx->theStringLen = len;
		thisx->val.theString = NEWARRAY(WCHAR, len+1);
		thisx->val.theString[len] = 0; /* NULL TERMINATED outside the asked for length */
		}
	else
		{
		Var_Empty(thisx);
		thisx->type = typeString;
		thisx->theStringLen = len;
		thisx->val.theString = NULL;
		}
}


VOID Var_AssignBool(pstructVar thisx, BOOL val)
{
	Var_Empty(thisx);
	thisx->type = typeBool;
	thisx->val.theInt = val;
}


VOID Var_AssignInvalid(pstructVar thisx)
{
	Var_Empty(thisx);
	thisx->type = typeInvalid;
	thisx->val.theInt = 0;
}

