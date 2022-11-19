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
#include "si_ops.h"

#include "si_types.h"
#include "si_misc.h"

/*==========================================
	OpS_New
============================================

---Purpose: 
To create and initialise an operand stack.
      
---Params:

               
---Return:
pstructOpS		the created OpS	
NULL					something went wrong

------------------------------------------------------------------------*/
pstructOpS OpS_New( VOID )
{
	pstructOpS	ops = NEWSTRUCT( structOpS );

	if (ops != NULL) {
		f_lh_newhead( &(ops->head) );

		if (ops->head == NULL) {
			DEALLOC( &ops );
		}
	}

	return ops;
}



/*==========================================
	OpS_NewFromOpS
============================================

---Purpose: 
To create and initialise an operand stack. The created OpS is initialised by moving entries from
another OpS.
      
---Params:
src						The OpS to move from
nbrToCopy			how many entries from the top of the stack to be moved.
								If -1 is passed in this argument all entries from src are moved.
               
---Return:
pstructOpS		the created OpS	
NULL					something went wrong

------------------------------------------------------------------------*/
pstructOpS OpS_NewFromOpS( pstructOpS src, INT16 nbrToMove )
/* nbrToMove == -1 means the whole OpS */
/* NULL is returned if something went wrong (e.g. nbrToMove > topIndex of src) */
{
	pstructOpS		result = NULL;
	t_link				link;
	UINT16				i;
	
	
	if (src == NULL)
		return NULL;

	if (nbrToMove == -1)
		nbrToMove = OpS_GetTopIndex(src);

	if (nbrToMove > OpS_GetTopIndex(src)) /* can't move more than the src holds */
		return NULL;

	result = OpS_New();
	if (result != NULL) {
		i = 0;
		while (i < nbrToMove) {
			link = f_lh_last( src->head );
			f_lh_out( &link );
			f_lh_into_as_first( &link, &(result->head) );
				/* the entries must be in the same order as in the src stack. 
					Therfore the entries are taken from the back and put in at the front.*/
			i++;
		}
	}
	return result;
}


/*==========================================
	OpS_NewFromOpS
============================================

---Purpose: 
To create and initialise an operand stack. The created OpS is initialised 
by copying entries from another OpS.
      
---Params:
src						The OpS to copy from
nbrToCopy			how many entries from the top of the stack to be copied.
								If -1 is passed in this argument all entries from src are copied.
               
---Return:
pstructOpS		the created OpS	
NULL					something went wrong

------------------------------------------------------------------------*/
VOID OpS_Delete( pstructOpS *pThis )
{
	t_link				link;
	pstructVar		aVar;

	if (*pThis != NULL) {
		while ( ! f_lh_empty((*pThis)->head) ) {
			link = f_lh_last((*pThis)->head);
			aVar = (pstructVar) link->data;
			Var_Delete( &aVar );
			f_lh_disposelink( &link );
		}
		f_lh_disposehead( &((*pThis)->head) );

		DEALLOC( pThis );
	}
}



/*==========================================
	OpS_Push
============================================

---Purpose: 
To push a var on an OpS.
	
---Params:
thisx			the OpS to operate on
pVar			the var to be push.
						The reference will afterwards be set to NULL to avoid
						misuse of the reference.
	
---Return:

------------------------------------------------------------------------*/
VOID OpS_Push( pstructOpS thisx, pstructVar *pVar )
{
	t_link	newEl = NULL;

	if ((thisx != NULL) && (*pVar != NULL)) {
		f_lh_newlink( &newEl );
		if (newEl != NULL) {
			newEl->data = *pVar;
			*pVar = NULL; /* so that the reference is not used by mistake somewhere else */
			f_lh_into( &newEl, &(thisx->head) );
		}
	}
}


/*==========================================
	OpS_Pop
============================================

---Purpose: 
To pop a var from an OpS.
	
---Params:
thisx					the OpS to operate on
	
---Return:
pstructVar		the top stack element
NULL					the stack is empty 

------------------------------------------------------------------------*/
pstructVar OpS_Pop( pstructOpS thisx )
{
	t_link				link;
	pstructVar		res = NULL;

	link = f_lh_last(thisx->head);
	if (link != NULL) {
		res = (pstructVar) link->data;
		f_lh_disposelink( &link );
	}

	return res;
}



/*==========================================
	OpS_GetTopIndex
============================================

---Purpose: 
To get the index of the top element 
(i.e. how many elements are in the stack).
Example: If one element is is the stack the topindex is 1.
If the stack is empty zero is returned.
	
---Params:
thisx					the OpS to operate on
	
---Return:
UINT16				the index of the top stack element

------------------------------------------------------------------------*/
UINT16 OpS_GetTopIndex( pstructOpS thisx )
/* top index is 0 if the stack is empty  */
{ 
	UINT16	res = 0;

	if (thisx != NULL) {
		if (thisx->head != NULL) {
			res = f_lh_cardinal( thisx->head );
		}
	}

	return res;
}


/*==========================================
	OpS_ClearUntilIndex
============================================

---Purpose: 
To pop stack entries until there is <index> entries left in the stack.
(Needed for cleaning up the OpS after upon function return statement)
	
---Params:
thisx					the OpS to operate on
index					which index the top entry in the stack must have after 
								the operation is done
	
---Return:
TRUE					the operation was successful
FALSE					the operation failed (e.g. less than <index> entries in the stack)

------------------------------------------------------------------------*/
BOOL OpS_ClearUntilIndex( pstructOpS thisx, UINT16 index )
{
	UINT16			topIndex = OpS_GetTopIndex(thisx);
	UINT16			nbrOfElToPop;
	UINT16			i;
	pstructVar	aVar;

	if (index > topIndex) {
		return FALSE;
	}
	else {
		nbrOfElToPop = topIndex - index;

		i=0;
		while (i < nbrOfElToPop) {
			aVar = OpS_Pop( thisx );
			Var_Delete( &aVar );
			i++;
		}

		return TRUE;
	}
}



/*==========================================
	OpS_NewFromArgString
============================================

---Purpose: 

	
---Params:

               
---Return:


------------------------------------------------------------------------*/
/* the states needed to parse the argstring */
typedef enum {
	argstate_start,
	argstate_nextOrDone,
	argstate_next,
	argstate_invalid,
	argstate_true,
	argstate_false,
	argstate_string,
	argstate_number,
	argstate_done,
	argstate_error
} enumArgState;


/*------------------------------------------------------*/


pstructOpS OpS_NewFromArgString( WCHAR* argString )
/*
The argString must start with a '(' and then ends parsing when ')' is found
*/
{
	pstructOpS		result = OpS_New();
	UINT32				strPos = 0;
	UINT32				argstrLen = 0;
	UINT32				argStart;
	UINT32				argEnd;
	WCHAR					aChar;
	enumArgState	state = argstate_start;
	pstructVar		anArgVar = NULL;

	BOOL							isSingleQuoteString = FALSE;	/* to know if the state argstate_string is contained within ' or " */
	UINT32						theNewStrLen;
	pstructEscapeSeq	escapeSeqList = NULL;
	pstructEscapeSeq	lastEscEl = NULL;
	pstructEscapeSeq	newEscEl = NULL;
	pstructEscapeSeq	escEl = NULL;
	UINT32						srcPos;
	UINT32						dstPos;


/* the following three vars are stack allocated and will be cleaned away by the system at function exit */
	WCHAR					invalidStr[]	=	{'i','n','v','a','l','i','d', 0};	/* needed to compare with argsString to see if found  */
	WCHAR					trueStr[]			= {'t','r','u','e', 0};			/* needed to compare with argsString to see if found  */
	WCHAR					falseStr[]		= {'f','a','l','s','e', 0};		/* needed to compare with argsString to see if found  */

	UINT32				invalidStrLen		= STRINGLENGTH( invalidStr );
	UINT32				trueStrLen			= STRINGLENGTH( trueStr );
	UINT32				falseStrLen			= STRINGLENGTH( falseStr );


	if (argString != NULL) {
		argstrLen = STRINGLENGTH( argString );
		while ((state != argstate_error) && (state != argstate_done) && (strPos < argstrLen)) 
		{
			aChar = argString[ strPos ];
			
			switch (state)
			{

			case argstate_start:	/* starts here on first char after opening parathesis */
				if (IsWhitespaceChar(aChar)) {
					/* nothing to be done except the strpos++ */
				}
				else if (IsEndParathesisChar(aChar)) {
					state = argstate_done;
				}
				else {
					if (IsNumberChar(aChar)) {
						state = argstate_number;
					}
					else if (IsSingleQuoteChar(aChar)) {
						theNewStrLen = 0;
						lastEscEl = NULL;
						newEscEl = NULL;
						isSingleQuoteString = TRUE;
						state = argstate_string;
					}
					else if (IsDoubleQuoteChar(aChar)) {
						theNewStrLen = 0;
						lastEscEl = NULL;
						newEscEl = NULL;
						isSingleQuoteString = FALSE;
						state = argstate_string;
					}
					else if (IsTrueStartChar(aChar)) {
						state = argstate_true;
					}
					else if (IsFalseStartChar(aChar)) {
						state = argstate_false;
					}
					else if (IsInvalidStartChar(aChar)) {
						state = argstate_invalid;
					}
					else {
						state = argstate_error;
					}
					argStart = strPos;
				}
				strPos++;
				break;

			case argstate_nextOrDone: /* waiting for comma or end paranthesis */
				if (IsWhitespaceChar(aChar)) {
					strPos++;
				}
				else if (IsCommaChar(aChar)) {
					state = argstate_next;
					strPos++;
				}
				else if (IsEndParathesisChar(aChar)) {
					state = argstate_done;
				}
				else {
					state = argstate_error;
				}
				break;

			case argstate_next:	/* waiting for start of next argument */
				if (IsWhitespaceChar(aChar)) {
					/* nothing to be done except the strpos++ */
				}
				else {
					if (IsNumberChar(aChar)) {
						state = argstate_number;
					}
					else if (IsSingleQuoteChar(aChar)) {
						theNewStrLen = 0;
						lastEscEl = NULL;
						newEscEl = NULL;
						isSingleQuoteString = TRUE;
						state = argstate_string;
					}
					else if (IsDoubleQuoteChar(aChar)) {
						theNewStrLen = 0;
						lastEscEl = NULL;
						newEscEl = NULL;
						isSingleQuoteString = FALSE;
						state = argstate_string;
					}
					else if (IsTrueStartChar(aChar)) {
						state = argstate_true;
					}
					else if (IsFalseStartChar(aChar)) {
						state = argstate_false;
					}
					else if (IsInvalidStartChar(aChar)) {
						state = argstate_invalid;
					}
					else {
						state = argstate_error;
					}
					argStart = strPos;
				}
				strPos++;
				break;

			case argstate_invalid:	/* 'i' has been found and the rest of the 'invalid' literal should come */
				/* check if the 'invalid' literal can fit in the rest of the argStr */
				if ((strPos + invalidStrLen -1) > argstrLen) {
					state = argstate_error;
					break;
				}
				/* now check if the 'invalid' literal is in the text, else an error has occured */
				if (COMPARESTRINGN( &(argString[argStart]), invalidStr, invalidStrLen ) == 0) {
					/* literal found! */
					strPos += (invalidStrLen-1);	/* -1 because the first letter has already been stepped past */
					/* push invalid on the OpS */
					anArgVar = Var_New();
					Var_AssignInvalid( anArgVar );
					OpS_Push( result, &anArgVar );
					state = argstate_nextOrDone;
				}
				else {
					state = argstate_error;
				}
				break;

			case argstate_true:	/* 't' has been found and the rest of the 'true' literal should come */
				/* check if the 'true' literal can fit in the rest of the argStr */
				if ((strPos + trueStrLen -1) > argstrLen) {
					state = argstate_error;
					break;
				}
				/* now check if the 'true' literal is in the text, else an error has occured */
				if (COMPARESTRINGN( &(argString[argStart]), trueStr, trueStrLen ) == 0) {
					/* literal found! */
					strPos += (trueStrLen-1);	/* -1 because the first letter has already been stepped past */
					/* push invalid on the OpS */
					anArgVar = Var_New();
					Var_AssignBool( anArgVar, TRUE );
					OpS_Push( result, &anArgVar );
					state = argstate_nextOrDone;
				}
				else {
					state = argstate_error;
				}
				break;

			case argstate_false:	/* 'f' has been found and the rest of the 'false' literal should come */
				/* check if the 'false' literal can fit in the rest of the argStr */
				if ((strPos + falseStrLen -1) > argstrLen) {
					state = argstate_error;
					break;
				}
				/* now check if the 'false' literal is in the text, else an error has occured */
				if (COMPARESTRINGN( &(argString[argStart]), falseStr, falseStrLen ) == 0) {
					/* literal found! */
					strPos += (falseStrLen-1);	/* -1 because the first letter has already been stepped past */
					/* push invalid on the OpS */
					anArgVar = Var_New();
					Var_AssignBool( anArgVar, FALSE );
					OpS_Push( result, &anArgVar );
					state = argstate_nextOrDone;
				}
				else {
					state = argstate_error;
				}
				break;

			case argstate_string:	/* inside a string */
				if ((IsSingleQuoteChar(aChar) && isSingleQuoteString) ||
						(IsDoubleQuoteChar(aChar) && (!isSingleQuoteString))) {
					/* has found the end of the string */
					argEnd = strPos;	/* the quotes are included in the argStart/argEnd */

					/* create a new string var with enough room for the parameter */
					anArgVar = Var_New();
					Var_NewString( anArgVar, theNewStrLen );

					srcPos = argStart+1;	/* the start of the the string contents is the first char after the start quote */
					dstPos = 0;
					escEl = escapeSeqList;
					while (escEl != NULL)
					{
						/* copy the chars up till the escape sequence */
						COPYSTRINGN( &(anArgVar->val.theString[dstPos]), &(argString[srcPos]), escEl->startPos - srcPos );
						dstPos += escEl->startPos - srcPos;
						/* insert the real char */
						anArgVar->val.theString[ dstPos ] = escEl->theChar;
						dstPos++;
						srcPos = escEl->endPos;

						escEl = escEl->next;
					}
					if (srcPos < argEnd)
					{	/* more of the string left, then copy the rest of the string */
						COPYSTRINGN( &(anArgVar->val.theString[dstPos]), &(argString[srcPos]), argEnd - srcPos );
					}
					/* the string parameter is done, now push it on the OpS */
					OpS_Push( result, &anArgVar );

					EscSeq_Delete( &escapeSeqList );
					/* clear the list of escape seq so that the next string doesn't get this already filled list */
					strPos++;
					state = argstate_nextOrDone;
				}
				else if (IsEscapeChar(aChar)) {
					strPos++;
					/* stepped over the "\" character */
					newEscEl = GetEscapedChar( argString, &strPos, argstrLen );
					if (newEscEl != NULL)
					{
						if (lastEscEl == NULL)
						{
							escapeSeqList = newEscEl;
						}
						else
						{
							lastEscEl->next = newEscEl;
						}
						lastEscEl = newEscEl;

						theNewStrLen++;
					}
					else
					{
						state = argstate_error;
					}
				}
				else {
					/* all other chars are just a part of the string */
					theNewStrLen++;	/* normal char adds one to the length */
					strPos++;
				}
				break;

			case argstate_number: /* inside a integer/float */
				if (IsNumberChar(aChar)) {
					/* still a number */
					strPos++;
				}
				else if (	IsWhitespaceChar(aChar) || 
									IsCommaChar(aChar) || 
									IsEndParathesisChar(aChar)) {
					/* 
					the number is now ended and must now be converted 
					to a pstructVar and pushed on the OpS 
					*/
					argEnd = strPos;
					anArgVar = Var_New();
					Var_AssignString( anArgVar, argEnd-argStart, &(argString[ argStart ]) );
					if (Var_ConvertMethod(CONVERT_INT_FLOAT, anArgVar, NULL)) {
						OpS_Push( result, &anArgVar );
						state = argstate_nextOrDone;
					}
					else {
						state = argstate_error;
					}
				}
				else {
					state = argstate_error;
				}
				/* 
				NOTE!!!: the strPos is not advanced, 
				the interpreting of the ',' or ')' or ' ' will be done in the next while run 
				*/
				break;
			}
		}

	}
	else {
		OpS_Delete( &result );
	}

	Var_Delete( &anArgVar );
	EscSeq_Delete( &escapeSeqList );	/* to make sure this has been deallocated */

	if (state == argstate_error) {
		OpS_Delete( &result );
	}

	return result;
}
