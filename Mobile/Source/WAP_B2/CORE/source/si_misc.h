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

#include "cmmnrsrc.h"
#include "si_var.h"
#include "WMLDef.h"


typedef struct StrFind 
{
	INT32							subStrPos;
	struct StrFind		*next;
	VOID*							data;		/* The pupose and use of this field is up to the user.
																Will be deallocated when StrFind_Delete is called */
	UINT32						endPos;	/* endPos is a field for future use and is not used by the 
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
str								the string which is to be examined for sub string matches
subStr						the str that is to be found
findAll						if TRUE all substring matches are reported, else only the first match
	
---Return:				
pstructStrFind		the first element in the list of found positions were the substring was found
NULL							the operation failed due to the variables were not of string type.

------------------------------------------------------------------------*/
pstructStrFind FindSubstrings( pstructVar str, pstructVar subStr, BOOL findAll );
VOID StrFind_Delete( pstructStrFind *pThis );


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
	UINT32	startPos;		/* the start position (in the argString) of the escape sequence */
	UINT32	endPos;			/* the end position (in the argString) of the escape sequence */
	WCHAR		theChar;		/* the actual unicode char that was coded by using an escape sequence */
	struct escapeSeq *next;
} structEscapeSeq, *pstructEscapeSeq;

pstructEscapeSeq GetEscapedChar( WCHAR *argString, UINT32 *strPos, UINT32 argstrLen );
VOID EscSeq_Delete( pstructEscapeSeq *list );




WCHAR* parseFormatSpecifier( WCHAR *formatStr, UINT32 strLen, UINT32 *pStrPos,
														pstructVar value, BOOL *pWentOk );




#ifdef HAS_FLOAT
BOOL IsLegalFloatVal( FLOAT32 a );
BOOL IsUnderflow( FLOAT32 a );
BOOL FloatIsZero( FLOAT32 a );
#endif
/* ifdef HAS_FLOAT */

#endif
/* define _SI_MISC_H */


