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
#ifndef _SI_MISC_H
#define _SI_MISC_H
/***********************
File: si_misc.h + c
Author: Klas Hermodsson (KHN)

Description:
Miscellaneous utility functions needed by si_* modules.

************************/

#include "cmmnrsrc.h"
#include "si_var.h"
#include "wmldef.h"


typedef struct StrFind 
{
  INT32             subStrPos;
  struct StrFind    *next;
  VOID*             data;   /* The pupose and use of this field is up to the user.
                                Will be deallocated when StrFind_Delete is called */
  UINT32            endPos; /* endPos is a field for future use and is not used by the 
                                FindSubstrings function */
} structStrFind, *pstructStrFind;


/*==========================================
  FindSubstrings
============================================

---Purpose: 
To find the positions in str were the subStr is located.
The positions is returned as a single linked list.
It is also possible to onlyfind the first substring match. 
This is done by calling the function with the argument findAll set to FALSE

---Params:
str               the string which is to be examined for sub string matches
subStr            the str that is to be found
findAll           if TRUE all substring matches are reported, else only the first match

---Return:
pstructStrFind    the first element in the list of found positions were the substring was found
NULL              the operation failed due to the variables were not of string type.

------------------------------------------------------------------------*/
pstructStrFind FindSubstrings( pstructVar str, pstructVar subStr, BOOL findAll );

/*==========================================
  StrFind_Delete
============================================

---Purpose: 
To deallocate the list of structStrFind structs.
*pThis is then set to NULL.

---Params:
pThis     the list of structStrFind structures that should be deallocated.

---Return:
-

------------------------------------------------------------------------*/
VOID StrFind_Delete( pstructStrFind *pThis );


/*
Misc functions that checks a unicode character if it is 
of a certain type.
*/

BOOL IsNumberChar(WCHAR aChar);
/* NOTE!!! This function does not return true for ".","e" or "E"
if HAS_FLOAT is 0 */

BOOL IsWhitespaceChar(WCHAR aChar);
BOOL IsInvalidStartChar(WCHAR aChar);
BOOL IsTrueStartChar(WCHAR aChar);
BOOL IsFalseStartChar(WCHAR aChar);
BOOL IsSingleQuoteChar(WCHAR aChar);
BOOL IsDoubleQuoteChar(WCHAR aChar);
BOOL IsFirstFunctionNameChar(WCHAR aChar);
BOOL IsFunctionNameChar(WCHAR aChar);
BOOL IsEndParathesisChar(WCHAR aChar);
BOOL IsCommaChar(WCHAR aChar);
BOOL IsEscapeChar(WCHAR aChar);


/* 
the list element data structure needed to keep track of escape sequences
inside string parameters in the argString.
*/
typedef struct escapeSeq {
  UINT32  startPos;   /* the start position (in the argString) of the escape sequence */
  UINT32  endPos;     /* the end position (in the argString) of the escape sequence */
  WCHAR   theChar;    /* the actual unicode char that was coded by using an escape sequence */
  struct escapeSeq *next;
} structEscapeSeq, *pstructEscapeSeq;

/*==========================================
  GetEscapedChar
============================================

---Purpose: 
To get the actual character from an escape sequence.
The escape sequences are listed in the WMLScript language 
specification Version 1.2 section 6.1.5.3.

---Params:
argString   The string in which an escape sequence start character (\) was found
strPos      The position in argString after a found \ character 
              (which is the start of an escape sequence).
              This parameter is changed by this function and
              after the call it will point to the character after the
              escape sequence.
argstrLen   The length, in number of characters, of argString.

---Return:
pstructEscapeSeq  If a correct escape sequence was found
NULL              If an incorrect escape sequence was found. Then the string
                    does not comply with WMLScript string format and should be
                    regarded as an invalid string value.

------------------------------------------------------------------------*/
pstructEscapeSeq GetEscapedChar( WCHAR *argString, UINT32 *strPos, UINT32 argstrLen );

/*==========================================
	EscSeq_Delete
============================================

---Purpose: 
To completely deallocate the structEscapeSeq list.
*list will then be set to NULL.

---Params:
list    The list of structEscapeSeq structs to be deallocated.

---Return:
-

------------------------------------------------------------------------*/
VOID EscSeq_Delete( pstructEscapeSeq *list );



/*==========================================
  parseFormatSpecifier
============================================

---Purpose: 
To produce output according to the format specifier in
formatStr. This function is used by the String.format
standard library implementation. It is to be called
when a % character has been found in the formatStr
and it is not followed by another % character (which is 
then the representation of a % character).

---Params:
formatStr   The string in which a format specifier is found.
strLen      The length, in number of characters, of formatStr.
pStrPos     The position in formatStr after the found  % character.
value       The value which is to be formatted in accordance with the format specifier. 
pWentOk     This parameter will be set to TRUE if no error occurred and FALSE else.

---Return:
WCHAR*      If *pWentOk == TRUE then the formatted result will be
            returned.

------------------------------------------------------------------------*/
WCHAR* parseFormatSpecifier( WCHAR *formatStr, UINT32 strLen, UINT32 *pStrPos,
                            pstructVar value, BOOL *pWentOk );




#ifdef HAS_FLOAT
/*
Misc functions to check the value of a FLOAT32 value.
*/
BOOL IsLegalFloatVal( FLOAT32 a );
BOOL IsUnderflow( FLOAT32 a );
BOOL FloatIsZero( FLOAT32 a );
#endif
/* ifdef HAS_FLOAT */

#endif
/* define _SI_MISC_H */


