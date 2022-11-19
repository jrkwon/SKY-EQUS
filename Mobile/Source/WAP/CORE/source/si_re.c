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
000329 (KHN) Changes due to function name change. ReadByte -> wip_ReadByte
**********/

#include "si_re.h"



/*==========================================
	RE_Init
============================================

---Purpose:
Helper function that helps with creating a runtime environment and initialising it.

---Params:
thisx					the RE to be initalised
BP						the bytecode package to be encapsulated by the RE
BPlen					the length of the BP in bytes
IANAcharset		string encoding within the BP file according to the WSP header
funcname			the name of the called function that caused this BP to load (not a local call), encoded in UTF8
refererUrl		the URL that referred to this script file
baseUrl				the URL to this script file

---Return:
TRUE					the RE was successfully initialised
FALSE					the RE could for some reason not be initialised

------------------------------------------------------------------------*/
enumErrorCode RE_Init( pstructRuntimeEnv thisx, BYTE* BP, UINT32 BPlen,
		INT16 IANAcharset, BYTE* funcname, /* funcname must be UTF8 encoded */
		WCHAR* refererUrl, WCHAR *baseUrl )
{
	UINT16						nbrOfArgs;
	UINT8							funcIndex;
	enumErrorCode			result = ERR_WAE_WMLS_NONE;

	if ((thisx != NULL) && (thisx->OpS != NULL))
	{

		nbrOfArgs = OpS_GetTopIndex( thisx->OpS );

		if (nbrOfArgs <= 0xFF)
		{
			thisx->BP = BP;
			thisx->BPend = BPlen;
			thisx->FS = FS_New();
			thisx->BPI = BPI_New( BP, BPlen, IANAcharset );

			if (thisx->BPI != NULL)
			{
				result = BPI_AccessCheck( thisx->BPI, BP, BPlen, funcname, (UINT8)nbrOfArgs, refererUrl, baseUrl, &funcIndex);

				if (result == ERR_WAE_WMLS_NONE)
				{
					thisx->currF = Frame_New( funcIndex, thisx->BPI, thisx->OpS );
						/* must send in the topstack number since the Frame_New subtracts this
					with nbrOfArgs to come up with the returnSP */

					if (thisx->currF != NULL)
					{
						thisx->IP = thisx->BPI->functions[ funcIndex ].startPos;

						return ERR_WAE_WMLS_NONE;
					}
					else
					{
						/* more arguments than there are operands on the OpS */
						return ERR_WAE_WMLS_STACK_UNDERFLOW;
					}
				}
				else
				{
					return result;
				}
			}
			else
			{
				return ERR_WAE_WMLS_VERIFICATION;
			}
		}
		else
		{
			return ERR_WAE_WMLS_VERIFICATION;
		}
	}
	else
	{
		return ERR_WAE_WMLS_VERIFICATION;
	}
}


/*==========================================
	RE_NewWithArgString
============================================

---Purpose:
To create a RE for the BP. Check that execution access is granted for the package.
Check that the called function exists and that the arguments passed comply.

The arguments for the function are passed in a ??? encoded string in the following format: ???.
	
---Params:
thisx					the RE to be initalised
BP						the bytecode package to be encapsulated by the RE
BPlen					the length of the BP in bytes
IANAcharset		string encoding within the BP file according to the WSP header
funcname			the name of the called function that caused this BP to load (not a local call), encoded in UTF8
argString			the string that contains the arguments for the function to be called.
refererUrl		the URL that referred to this script file
baseUrl				the URL to this script file

---Return:
pstructRuntimeEnv		the RE was successfully created and initalised
NULL								something went wrong

------------------------------------------------------------------------*/
pstructRuntimeEnv RE_NewWithArgString( BYTE* BP, UINT32 BPlen, INT16 IANAcharset, BYTE* funcname, /* funcname must be UTF8 encoded */
		WCHAR* argString, WCHAR* refererUrl, WCHAR *baseUrl, enumErrorCode *errCode )
{
	pstructRuntimeEnv			result = NEWSTRUCT(structRuntimeEnv);

	result->BP		= NULL;
	result->BPI		= NULL;
	result->OpS		= NULL;
	result->FS		= NULL;
	result->currF = NULL;

	result->OpS = OpS_NewFromArgString( argString );
	if (result->OpS != NULL) {
		if ( (*errCode = RE_Init( result, BP, BPlen, IANAcharset, funcname, refererUrl, baseUrl )) == ERR_WAE_WMLS_NONE ) {
			return result;
		}
	}
	else
	{
		*errCode = ERR_WAE_WMLS_FUNC_ARGS;
	}

	/* if the function get to this line then something has gone wrong and the RE must be deallocated */
	RE_Delete( &result );
	return NULL;
}


/*==========================================
	RE_NewWithArgOpS
============================================

---Purpose:
To create a RE for the BP. Check that execution access is granted for the package.
Check that the called function exists and that the arguments passed comply.

The arguments for the function are passed in an operand stack (OpS). The "argument OpS" must be created for
this RE especially since it will become the OpS of this new RE.

---Params:
thisx					the RE to be initalised
BP						the bytecode package to be encapsulated by the RE
BPlen					the length of the BP in bytes
IANAcharset		string encoding within the BP file according to the WSP header
funcname			the name of the called function that caused this BP to load (not a local call)
ops						an OpS that contains the arguments for the function to be called.
								This OpS will become the new OpS of the RE that is to be created.
refererUrl		the URL that referred to this script file
baseUrl				the URL to this script file

---Return:
pstructRuntimeEnv		the RE was successfully created and initalised
NULL								something went wrong

------------------------------------------------------------------------*/
pstructRuntimeEnv RE_NewWithArgOpS( BYTE* BP, UINT32 BPlen, INT16 IANAcharset, BYTE* funcname,
		pstructOpS* ops, WCHAR* refererUrl, WCHAR *baseUrl, enumErrorCode *errCode )
{
	pstructRuntimeEnv			result = NEWSTRUCT(structRuntimeEnv);

	result->BP		= NULL;
	result->BPI		= NULL;
	result->OpS		= NULL;
	result->FS		= NULL;
	result->currF = NULL;

	result->OpS = *ops;
	*ops = NULL;	/* so the variable is not used later by mistake */
	if (result->OpS != NULL) {
		if ( (*errCode = RE_Init( result, BP, BPlen, IANAcharset, funcname, refererUrl, baseUrl )) == ERR_WAE_WMLS_NONE ) {
			return result;
		}
	}

	/* if the function get to this line then something has gone wrong and the RE must be deallocated */
	RE_Delete( &result );
	return NULL;
}


/*==========================================
	RE_Delete
============================================

---Purpose: 
To delete the complete RE and the structuresit contains.
The reference to the RE that is passed will be set to NULL afterwards
to make misuse impossible.
	
---Params:
pThis					a reference to the RE to be deleted.
               
---Return:


------------------------------------------------------------------------*/
VOID RE_Delete( pstructRuntimeEnv *pThis )
{
	if (*pThis != NULL) {
		/* NOTE:  */
		FS_Delete( &( (*pThis)->FS ), (*pThis)->BPI );
		Frame_Delete( &( (*pThis)->currF ), (*pThis)->BPI );
		BPI_Delete( &( (*pThis)->BPI ) );
		OpS_Delete( &( (*pThis)->OpS ) );

		DEALLOC( pThis );
	}
}



/*==========================================
	RE_GetResult
============================================

---Purpose: 
To pop the operand stack (OpS) and return it as the result.
If this function is called after the package is done, the result is the top OpS element.

---Params:
This					the RE to perform this operation on
               
---Return:
pstructVar		the result as a WMLS var
NULL					something went wrong (e.g. an empty stack)

------------------------------------------------------------------------*/
pstructVar RE_GetResult( pstructRuntimeEnv thisx ) /* pops the top operand from the OpS */
{
	if ( (thisx != NULL) && (thisx->OpS != NULL) ) {
		return OpS_Pop( thisx->OpS );
	}
	else return NULL;
}



/*==========================================
	RE_ReadInstruction
============================================

---Purpose: 
To read the next instruction from the BP and advances the IP.
This operation checks that the IP does not pass the end of the current function.
The result is returned in one of the parameters.

---Params:
This					the RE to perform this operation on
result				the result of this operation will be put in this variable
               
---Return:
TRUE					everything went ok
FALSE					Something went wrong

------------------------------------------------------------------------*/
BOOL RE_ReadInstruction( pstructRuntimeEnv thisx, BYTE* result )
{
	UINT32			funcEndPos;

	if ( (thisx != NULL) && (thisx->currF) ) {
		funcEndPos = thisx->BPI->functions[ thisx->currF->funcIndex ].endPos;

		return wip_ReadByte( result, thisx->BP, funcEndPos, &(thisx->IP) );
	}

	return FALSE;
}


/*==========================================
	RE_Read8bOperand
============================================

---Purpose: 
To read the 8 bit operand to from the BP and advance the IP.
The IP is checked so that it does not pass the end of the current function.
The result is returned in one of the parameters.

---Params:
This					the RE to perform this operation on
result				the result of this operation will be put in this variable
               
---Return:
TRUE					everything went ok
FALSE					Something went wrong

------------------------------------------------------------------------*/
BOOL RE_Read8bOperand( pstructRuntimeEnv thisx, UINT8* result )
{
	UINT32			funcEndPos;

	if ( (thisx != NULL) && (thisx->currF) ) {
		funcEndPos = thisx->BPI->functions[ thisx->currF->funcIndex ].endPos;

		return ReadUInt8( result, thisx->BP, funcEndPos, &(thisx->IP) );
	}

	return FALSE;
}


/*==========================================
	RE_Read16bOperand
============================================

---Purpose: 
To read the 16 bit operand to from the BP and advance the IP.
The IP is checked so that it does not pass the end of the current function.
The result is returned in one of the parameters.

---Params:
This					the RE to perform this operation on
result				the result of this operation will be put in this variable
               
---Return:
TRUE					everything went ok
FALSE					Something went wrong

------------------------------------------------------------------------*/
BOOL RE_Read16bOperand( pstructRuntimeEnv thisx, UINT16* result )
{
	UINT32			funcEndPos;

	if ( (thisx != NULL) && (thisx->currF) ) {
		funcEndPos = thisx->BPI->functions[ thisx->currF->funcIndex ].endPos;

		return ReadUInt16( result, thisx->BP, funcEndPos, &(thisx->IP) );
	}

	return FALSE;
}


/*==========================================
	RE_Jump
============================================

---Purpose:
To offset the IP (IP + offset). NOTE: offset can be a negative integer.
A check is performed so that the destination is within the current function.

Note: The jump is still valid if the destination is the byte after the end of
the function. This will mean that an implicit RETURN_ES will be performed.
(end note (990104, KHN))

---Params:
This					the RE to perform this operation on
offset				the number of bytes to offset the IP by

---Return:
TRUE					everything went ok
FALSE					Something went wrong

------------------------------------------------------------------------*/
BOOL RE_Jump( pstructRuntimeEnv thisx, INT32 offset )
{
	UINT32 destPos;

	if ( (offset < 0) && ((INT32)thisx->IP < -offset) ) {
		/* if offset is not less than -IP because
		then a not allowed bw jump can become valid due to aclc error (unsigned) */
		return FALSE;
	}

	destPos = thisx->IP + offset;

	if (	(destPos >= thisx->BPI->functions[ thisx->currF->funcIndex ].startPos ) && 
				(destPos <=  thisx->BPI->functions[ thisx->currF->funcIndex ].endPos ) ) {
		thisx->IP  = destPos;
		return TRUE;
	}

	return FALSE;
}



/*==========================================
	RE_Pop
============================================

---Purpose: 
To perform a safe pop from the operand stack. Safe means that
the current frame's stack space will not be underflowed.

---Params:
This					the RE to perform this operation on
	
---Return:
pstructVar		the top stack element
NULL					the stack has reached it's bottom

------------------------------------------------------------------------*/
pstructVar RE_Pop( pstructRuntimeEnv thisx )
{
	if (OpS_GetTopIndex( thisx->OpS ) > thisx->currF->returnSP) {
		return OpS_Pop( thisx->OpS );
	}
	else {
		return NULL;
	}
}

