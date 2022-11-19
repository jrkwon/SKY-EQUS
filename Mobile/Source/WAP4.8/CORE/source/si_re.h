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

/***********************
File: si_re.h + c
Author: Klas Hermodsson (KHN)

Description:
RE stands for runtime environment. There exists one and only one RE per SI (see si_si).
Thus, one RE corresponds to one script file.
The runtime environment consists of the following components:

BP - bytecode package
The actual compiled script that is to be executed. 

BPend
Length of the BP. BP + BPend results in a pointer to the byte just after the
BP.

BPI - BP information
This structure is created by traversing the BP above and storing important 
positions in the BP so we later can quickly access information in the BP
without traversing the whole BP again.
For more information see si_bpi.

OpS - Operand stack
The operand stack for the runtime environment.
For more information see si_ops.

FS - frame stack
A frame contains the context information about a function. Thus, the frame stack
is the call stack of functions in the current script file.
For more information about the frame stack see si_fs.
For more information about the frame see si_frame.

IP - instruction pointer
The IP points out the current position of execution in the BP. 
The IP is not a memory pointer but an offset. To get the memory pos of the current instruction 
one must add the IP to the BP pointer.

currF - current frame
The current function context.
For more information on frames see si_frame.


Tasks of the RE:
Creation of the RE from a script file and a call to a function within the file.
A RE can be created in two different ways.
* A script function is called from WML.
    A new script call is being made. The function call with
		function name and parameters are contained in the fragment of the
		call URL.
* A script function is called from another script file.
    The script call is being made from an already running 
		script file to a function in another script file.
		The function name exists as a string and the parameters in the 
		call are already in the internal format (an operand stack 
		filled with the parameter values).
		
The RE is also used for changing the IP
i.e: 
   * Read an instruction in the byte code
   * Offset the IP (perform a jump in the byte code)
All the IP changing operations are "safe" (i.e. checks are made to ensure that the IP doesn't
move outside the boundaries of the current function).

Some OpS operations are also performed by the RE. This is to be able to perform checks so
that the code doesn't pop operands beyond the stack bottom of the current function.

************************/

#ifndef _SI_RE_H
#define _SI_RE_H


#include "cmmnrsrc.h"
#include "si_bpi.h"
#include "si_ops.h"
#include "si_frame.h"
#include "si_fs.h"
#include "si_var.h"


typedef struct
{
	BYTE*					BP;
	UINT32				BPend; /* (BP offset that points to the byte after the last byte of the BP */
	pstructBPInfo	BPI;
	pstructOpS		OpS;
	pstructFS			FS;
	UINT32				IP; /* BP offset */
	pstructFrame	currF;
} structRuntimeEnv, *pstructRuntimeEnv;


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
		WCHAR* argString, WCHAR* refererUrl, WCHAR *baseUrl, enumErrorCode *errCode );

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
		pstructOpS* ops, WCHAR* refererUrl, WCHAR *baseUrl, enumErrorCode *errCode );
/* the ops given is taken over by the re (NOT COPIED!!!) */

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
VOID RE_Delete( pstructRuntimeEnv *pThis);

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
pstructVar RE_GetResult( pstructRuntimeEnv thisx ); /* pops the top operand from the OpS */

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
BOOL RE_ReadInstruction( pstructRuntimeEnv thisx, BYTE* result );

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
BOOL RE_Read8bOperand( pstructRuntimeEnv thisx, UINT8* result );

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
BOOL RE_Read16bOperand( pstructRuntimeEnv thisx, UINT16* result );

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
BOOL RE_Jump( pstructRuntimeEnv thisx, INT32 offset );

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
pstructVar RE_Pop( pstructRuntimeEnv thisx );

/* _SI_RE_H */
#endif
