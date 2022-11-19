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
               Behaviour of an URL call with a float argument when it
               is an integer only device.
990526 (KHN) Corrigendum WAG-WMLScript Standard Libraries-31
               String.format() behaviour in some cases.
990623 (KHN) The formatting of values from a specifier
               (used by the library function String.format) has been corrected
               on a number of details.
               * "%.f" now gives precision 0
               * a float with less significands than is to be printed is now
                  printed correct.
               * Whitespace in a specifier is not allowed.
               (function: parseFormatSpecifier)
990825 (KHN) Changed so that the parsing of float in String.format is done
               using sprintf.
               (function: parseFormatSpecifier)
991207 (KHN) WAP 1.2 update:
                SEC-18-Jun-1999-5 String.format on devices that doesn't support floating-point.
                Change: %f specifier when not supporting float => invalid.
000126 (KHN) Corrected the underlying function that performs the task of finding substrings. 
                The error was that if searching for example "BCD" in the "aaBBCD" 
                string resultet in no match.
000502 (KHN) Removed a warning found by GBU.
000802 (KHN) Changed to a safer sprintf calling style.
000808 (KHN) WAP 1.2.1 update:
                CR WMLSL-IBM-20000315-FormatConversion
                If doing a %d but the value is float then do Float.int() conversion first.
010108 (NKE) Rewritten FindSubstrings; "abac" didn't match "ababac".

**********/
#include "si_misc.h"


#include "si_types.h"
#include "si_int.h"

#ifdef HAS_FLOAT
  #include "si_float.h"
#endif

#include "url.h"
#include "wmlif.h"
#include "ansilibs.h"



/*==========================================
  FindSubstrings
============================================

---Purpose: 
To find the positions in str were the subStr is located.
The positions is returned as a single linked list.
It is also possible to onlyfind the first substring match. 
This is done by calling the function with the argument findAll set to FALSE.

---Params:
str               the string which is to be examined for sub string matches
subStr            the str that is to be found
findAll           if TRUE all substring matches are reported, else only the first match

---Return:
pstructStrFind    the first element in the list of found positions were the substring was found
NULL              the operation failed due to the variables were not of string type.

------------------------------------------------------------------------*/
pstructStrFind FindSubstrings( pstructVar str, pstructVar subStr, BOOL findAll )
{
	pstructStrFind  result = NULL;
	pstructStrFind  prev   = NULL;
	pstructStrFind  curr;
	UINT32          i;
	UINT32          j;

	if ( (VCR_OK != Var_Convert( str, typeString )) || (VCR_OK != Var_Convert( subStr, typeString )))
		return NULL;

	if (str->theStringLen > 0 && subStr->theStringLen > 0) 
		for (i = 0; i <= str->theStringLen - subStr->theStringLen; i++) 
			for (j = 0; str->val.theString[i+j] == subStr->val.theString[j]; j++)
				if (j == subStr->theStringLen - 1) {
					curr = NEWSTRUCT( structStrFind );
					curr->next = NULL;
					curr->data = NULL;
					curr->endPos = 0;
					curr->subStrPos = i;
					
					if (! findAll)
						return curr;

					i += j;
					if (prev == NULL) {
						prev = curr;
						result = curr;
					} else {
						prev->next = curr;
						prev = curr;
					}
					break;
				}
	
	if (result == NULL) { /* No match found */
		result = NEWSTRUCT( structStrFind );
		result->subStrPos = -1;
		result->next = NULL;
		result->data = NULL;
		result->endPos = 0;
	}

	return result;
}



VOID StrFind_Delete( pstructStrFind *pThis ) 
{
	if (*pThis != NULL) 
	{
		if ((*pThis)->next == NULL) 
		{
			DEALLOC( &((*pThis)->data) );
			DEALLOC( pThis );
		}
		else
		{
			StrFind_Delete( &((*pThis)->next) );
			DEALLOC( &((*pThis)->data) );
			DEALLOC( pThis );
		}
	}
}


/**/
BOOL IsNumberChar(WCHAR aChar)
{
/* NOTE!!! This function does not return true for ".","e" or "E"
if HAS_FLOAT is 0 */
#ifdef HAS_FLOAT
	return (	((aChar >= '0') && (aChar <= '9')) ||
						(aChar == '+') ||
						(aChar == '-') ||
						(aChar == '.') ||
						(aChar == 'e') ||
						(aChar == 'E'));
#else
	return (	((aChar >= '0') && (aChar <= '9')) ||
						(aChar == '+') ||
						(aChar == '-'));
#endif
}


BOOL IsWhitespaceChar(WCHAR aChar)
{
	return ( ((aChar >= CHAR_HORIZONTAL_TAB) && (aChar <= CHAR_CARRIAGE_RETURN))
  				 || (aChar == CHAR_SPACE) );
}


BOOL IsInvalidStartChar(WCHAR aChar)
{
	return ( aChar == 'i' );
}


BOOL IsTrueStartChar(WCHAR aChar)
{
	return ( aChar == 't' );
}


BOOL IsFalseStartChar(WCHAR aChar)
{
	return ( aChar == 'f' );
}


BOOL IsSingleQuoteChar(WCHAR aChar)
{
	return ( aChar == '\'' );
}


BOOL IsDoubleQuoteChar(WCHAR aChar)
{
	return ( aChar == '"' );
}


BOOL IsFirstFunctionNameChar(WCHAR aChar)
{
	return (	((aChar >= 'a') && (aChar <= 'z')) ||
						((aChar >= 'A') && (aChar <= 'Z')) ||
						(aChar == '_') );
}


BOOL IsFunctionNameChar(WCHAR aChar)
{
	return (	((aChar >= 'a') && (aChar <= 'z')) ||
						((aChar >= 'A') && (aChar <= 'Z')) ||
						(aChar == '_') || 
						((aChar >= '0') && (aChar <= '9')));
}


BOOL IsEndParathesisChar(WCHAR aChar)
{
	return ( aChar == ')' );
}


BOOL IsCommaChar(WCHAR aChar)
{
	return ( aChar == ',' );
}


BOOL IsEscapeChar(WCHAR aChar)
{
	return ( aChar == '\\' ); /* '\' */
}


INT32 GetSingleEscapeChar(WCHAR aChar)
{
	if ((aChar == '\\') ||	/* \ */
			(aChar == '/')	||	/* / */
			(aChar == '\'') ||	/* ' */
			(aChar == '"')			/* " */)
	{
		return aChar;
	}
	else if (aChar == 'b')
	{
		return 0x08;
	}
	else if (aChar == 'f')
	{
		return 0x0C;
	}
	else if (aChar == 'n')
	{
		return 0x0A;
	}
	else if (aChar == 'r')
	{
		return 0x0D;
	}
	else if (aChar == 't')
	{
		return 0x09;
	}
	else
	{
		return -1;
	}
}


INT8 GetHexValueFromChar( WCHAR aChar )
{
	if ((aChar >= '0') && (aChar <= '9'))
	{
		return (aChar - '0');
	}
	else if ((aChar >= 'a') && (aChar <= 'f'))
	{
		return (aChar - 'a' + 10);
	}
	else if ((aChar >= 'A') && (aChar <= 'F'))
	{
		return (aChar - 'A' + 10);
	}
	else
	{
		return -1;
	}
}

INT8 GetOctalValueFromChar( WCHAR aChar )
{
	if ((aChar >= '0') && (aChar <= '7'))
	{
		return (aChar - '0');
	}
	else
	{
		return -1;
	}
}


typedef enum
{
	esc_none,
	esc_octal,
	esc_hex,
	esc_unicode

} enumEscType;

pstructEscapeSeq GetEscapedChar( WCHAR *argString, UINT32 *strPos, UINT32 argstrLen )
{
	UINT32							startPos = (*strPos) - 1;
	
	pstructEscapeSeq		result = NULL;
	BOOL								done = FALSE;
	BOOL								error	= FALSE;
	enumEscType					escType	= esc_none;
	UINT32							nbrOfChars = 0;
	WCHAR								theResultChar = 0;

	INT8								val;

	WCHAR								aChar = 0;

	while ((*strPos < argstrLen) && (!done))
	{
		aChar = argString[ *strPos ];	
		switch (escType)
		{
			case esc_none:
				if (GetSingleEscapeChar(aChar) != -1 )
				{
					theResultChar = (UINT16)GetSingleEscapeChar(aChar);
					(*strPos)++;
					done = TRUE;
				}
				else if (aChar == 'x')
				{
					(*strPos)++;
					escType = esc_hex;
				}
				else if (aChar == 'u')
				{
					(*strPos)++;
					escType = esc_unicode;
				}
				else /* octal */
				{
					escType = esc_octal;
				}
				break;
			case esc_octal:
				if ((val = GetOctalValueFromChar(aChar)) != -1)
				{
					theResultChar = theResultChar << 3;
					theResultChar += val;
					(*strPos)++;
					nbrOfChars++;
					if (nbrOfChars == 3)
					{
						done = TRUE;
						if (theResultChar > 0xFF)
						{
							error = TRUE;
						}
					}
				}
				else
				{
					done = TRUE;
					if (nbrOfChars == 0)
					{
						error = TRUE;
					}
				}
				break;
			case esc_hex:
				if ((val = GetHexValueFromChar(aChar)) != -1)
				{
					theResultChar = theResultChar << 4;
					theResultChar += val;
					(*strPos)++;
					nbrOfChars++;
					if (nbrOfChars == 2)
					{
						done = TRUE;
					}
				}
				else
				{
					done = TRUE;
					error = TRUE;
				}
				break;
			case esc_unicode:
				if ((val = GetHexValueFromChar(aChar)) != -1)
				{
					theResultChar = theResultChar << 4;
					theResultChar += val;
					(*strPos)++;
					nbrOfChars++;
					if (nbrOfChars == 4)
					{
						done = TRUE;
					}
				}
				else
				{
					done = TRUE;
					error = TRUE;
				}
				break;
		}
	}

	if (done && (!error))
	{
		result = NEWSTRUCT( structEscapeSeq );
		result->startPos = startPos;
		result->endPos   = *strPos;
		result->theChar  = theResultChar;
		result->next = NULL;
	}

	return result;
}



VOID EscSeq_Delete( pstructEscapeSeq *list )
{
	pstructEscapeSeq	el = NULL;
	pstructEscapeSeq	nextEl = NULL;

	el = *list;
	while (el != NULL)
	{
		nextEl = el->next;
		DEALLOC( &el );
		el = nextEl;
	}
	*list = NULL;
}

/**/

typedef enum
{
	formatTypeNone = 1,
	formatTypeInt,
	formatTypeFloat,
	formatTypeString

} enumFormatType;



WCHAR* parseFormatSpecifier( WCHAR *formatStr, UINT32 strLen, UINT32 *pStrPos,
														pstructVar value, BOOL *pWentOk )
{
	WCHAR			*result = NULL;
	WCHAR			aChar = 0;
	BOOL			done = FALSE;
	UINT8			charsUsed = 0;
	INT32			number = 0;
	BOOL			overflow = FALSE;
	UINT32		i;
	UINT32		j;
	INT32			width = 0;
	INT32			precision = -1;
	enumFormatType	theFormatType = formatTypeNone;
	BOOL			aNegativeNumber = FALSE;
	BOOL			widthParsed     = FALSE;
	BOOL			precisionParsed = FALSE;
	BOOL			typeParsed      = FALSE;
  BYTE*			pchFloatAsString = NULL;
	*pWentOk = TRUE;

	while ((*pStrPos < strLen) && (!done))
	{
		aChar = formatStr[ *pStrPos ];

/*  
		if (IsWhitespaceChar(aChar))
		{
			(*pStrPos)++;
		}
		else
*/
    if (((aChar >= '0') && (aChar <= '9')) && (! widthParsed))
		{
			if (String2Int( formatStr + *pStrPos, &charsUsed, &number, &overflow ) &&
					((*pStrPos + charsUsed) < strLen))
			{
				*pStrPos += charsUsed;
				width = number;
			}
			else
			{
				done = TRUE;
				*pWentOk = FALSE;
			}
		}
		else if ((aChar == '.') && (! precisionParsed))
		{
      precisionParsed = TRUE;
			(*pStrPos)++;
			if (*pStrPos < strLen)
			{
				aChar = formatStr[ *pStrPos ];
				if ((aChar >= '0') && (aChar <= '9'))
				{
					if (String2Int( formatStr + *pStrPos, &charsUsed, &number, &overflow ) &&
							((*pStrPos + charsUsed) < strLen))
					{
						*pStrPos += charsUsed;
						precision = number;
					}
					else
					{
						done = TRUE;
						*pWentOk = FALSE;
					}
				}
			}
			else
			{
				done = TRUE;
				*pWentOk = FALSE;
			}
		}
		else if ((aChar == 'd') && (! typeParsed))
		{
			(*pStrPos)++;
			theFormatType = formatTypeInt;
			done = TRUE;
		}
		else if ((aChar == 'f') && (! typeParsed))
		{
			(*pStrPos)++;
			theFormatType = formatTypeFloat;
			done = TRUE;
		}
		else if ((aChar == 's') && (! typeParsed))
		{
			(*pStrPos)++;
			theFormatType = formatTypeString;
			done = TRUE;
		}
		else
		{
			done = TRUE;
			*pWentOk = FALSE;
		}
	}

	if (*pWentOk)
	{
		/* now format the value variable as specified */
	
		switch (theFormatType)
		{
			case formatTypeInt:

#ifdef HAS_FLOAT
/*      
        000808 (KHN) WAP 1.2.1 update:
           CR WMLSL-IBM-20000315-FormatConversion
           If doing a %d but the value is float then do Float.int() conversion first.
*/
				if (value->type == typeFloat) 
				{
					if ((value->val.theFloat >= FLOAT32_INTMAX_AS_FLOAT) || (value->val.theFloat < INT32_MIN) )
					{
						Var_AssignInvalid( value ); /*the float is bigger/smaller than an int can be*/
					}
					else 
					{
						Var_AssignInt( value, (INT32) value->val.theFloat );
					}
				}
#endif
				
				*pWentOk = (VCR_OK == Var_Convert( value, typeInteger ));
				break;

#ifdef HAS_FLOAT
			case formatTypeFloat:
				*pWentOk = (VCR_OK == Var_Convert( value, typeFloat ));
				break;
#else
			case formatTypeFloat:
				*pWentOk = FALSE;
				break;
#endif

			case formatTypeString:
				*pWentOk = (VCR_OK == Var_Convert( value, typeString ));
				break;

			default:
				*pWentOk = FALSE;
				break;
		}

		if (*pWentOk)
		{
			/* now convert to string while applying the width and precision variables */

			switch (value->type)
			{
				case typeInteger:
					aNegativeNumber = value->val.theInt < 0;

					if ((precision == 0) && (value->val.theInt == 0))
					{ /* the result is an empty string */
						*pWentOk = TRUE;
						/* totally done! The result will be NULL */
					}
					else if ( (*pWentOk = (VCR_OK == Var_Convert( value, typeString ))) )
					{
						if (precision == -1)
						{	/* no precision specified */
							precision = MAX( (INT32)value->theStringLen, precision );
						}
						else
						{
							if (! aNegativeNumber)
							{
								precision = MAX( (INT32)value->theStringLen, precision );
							}
							else
							{	/* if the number is negative the prcecision 3 will make "-xx" to become "-0xx" 
									thus the stringlength will be increased by one. To reflect this we
									add one to precision in this case. The precision is now the length of the number
									subject to the precision field. */
								precision = MAX( (INT32)value->theStringLen, precision + 1 );
							}
						}
						width = MAX( precision, width );

						if (width == (INT32)value->theStringLen)
						{	/* the result is the integer without adjustment */
							result = NEWARRAY( WCHAR, value->theStringLen + 1 );
							result[value->theStringLen] = 0;
							COPYSTRINGN( result, value->val.theString, value->theStringLen );
							/* done! */
							*pWentOk = TRUE;
						}
						else
						{ /* the string must be altered in some way */
							result = NEWARRAY( WCHAR, width + 1 );
							result[width] = 0;

							for (i=0; i < (UINT32)(width - precision); i++)
							{
								result[i] = CHAR_SPACE;
							}
							if (aNegativeNumber)
							{
								/* transfer the minus ('-') sign first before padding the number according to the precision */
								result[i] = value->val.theString[0];
								i++;
							}
							j = 0;
							while (j < ((UINT32)(precision - value->theStringLen)))
							{	/* pad with zeroes */
								result[i] = WC('0'); /* a zero */

								i++;
								j++;
							}
							if (aNegativeNumber)
							{
								COPYSTRINGN( result + i, value->val.theString + 1, value->theStringLen - 1 );
							}
							else
							{
								COPYSTRINGN( result + i, value->val.theString, value->theStringLen );
							}
							/* done! */
						}
					}
					break;

#ifdef HAS_FLOAT
				case typeFloat:
					if (precision == -1)
					{
						if (precisionParsed)
            {	/* only a dot but no number means precision 0 */
							precision = 0;
            }
            else
            { /* no precision given and the default is 6 */
							precision = 6;
            }
					}

					pchFloatAsString = NEWARRAY( BYTE, MAX( 41 + precision, width ) +1 );
#ifdef FEATURE_WAP
          sprintf ((char *)pchFloatAsString, "%*.*f", (int)width, (int)precision, (double)(value->val.theFloat) );
#else
          sprintf (pchFloatAsString, "%*.*f", (int)width, (int)precision, (double)(value->val.theFloat) );
#endif //#ifdef FEATURE_WAP
          result = wip_byte2wchar( pchFloatAsString );
					DEALLOC( &pchFloatAsString );
					break;
#endif

				case typeString:
					if (precision == -1)
					{
						precision = (INT32)value->theStringLen;
						width = MAX( precision, width );
					}
					else
					{
						precision = MIN( (INT32)value->theStringLen, precision );
						width = precision;
            /* When the width is larger than precision,
            the width should be ignored. Also, the width never trucates a value
            which means that it can never be less than precision. So if
            a precision is given, the width value is not important. */
					}

					result = NEWARRAY( WCHAR, width + 1 );
					result[width] = 0;

					for (i=0; i < (UINT32)(width - precision); i++)
					{
						result[i] = CHAR_SPACE;
					}
					COPYSTRINGN( result + i, value->val.theString, precision );
					break;

				default:
					*pWentOk = FALSE;
					break;
			}
		}
	}

	return result;
}



#ifdef HAS_FLOAT
BOOL IsLegalFloatVal( FLOAT32 a )
{
	UINT32 aAsHex = (* (UINT32*)((void*)&a) );

	if ( ((aAsHex) == 0xFF800000) ||  /* -INF */
  		 ((aAsHex) == 0x7F800000) ||  /* +INF */
       ( (((aAsHex) & 0x7F800000)== 0x7F800000) && ((aAsHex) & 0x007FFFFF) ) ) /* NaN */
  {
  	return FALSE;
  }
  else
  {
  	return TRUE;
  }
}

BOOL IsUnderflow( FLOAT32 a )
{
	if ((a > 0.0) && (a < FLOAT32_MIN))
  {
		/* underflow */
  	return TRUE;
  }
  else if ((a < 0.0) && (a > -FLOAT32_MIN))
  {
		/* underflow */
  	return TRUE;
  }
  else
  {
  	return FALSE;
  }
}


BOOL FloatIsZero( FLOAT32 a )
{
	UINT32 aAsHex = (* (UINT32*)((void*)&a) );

 	if ((aAsHex & 0x7FFFFFFF) == 0)
  {
  	return TRUE;
  }
  else
  {
  	return FALSE;
  }
}
#endif
/* ifdef HAS_FLOAT */

