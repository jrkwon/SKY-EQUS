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
#include "si_instr.h"

#include "si_frame.h"
#include "si_libs.h"
#include "url.h"


typedef struct {
	enumErrorCode		(*func)( BYTE, pstructSI ); /* pointer to a function */
	UINT16			timeCost;
} structInstr_S;

typedef struct {
	enumErrorCode		(*func)( pstructSI );
	UINT16			timeCost;
} structInstr;


/* Global instruction arrays */


const structInstr_S			array1XXPPPPP[4] = 
{
	{	Exec_JUMP_FW_S, 1},
	{	Exec_JUMP_BW_S, 1},
	{	Exec_TJUMP_FW_S, 1},
	{	Exec_LOAD_VAR_S, 1}
};

 
const structInstr_S			array010XPPPP[2] =
{
	{	Exec_STORE_VAR_S, 1},
	{	Exec_LOAD_CONST_S, 1}
};


const structInstr_S			array011XXPPP[4] =
{
	{	Exec_CALL_S, 1},
	{	Exec_CALL_LIB_S, 1},
	{	Exec_INCR_VAR_S, 1},
	{ NULL, 0}
};


const structInstr				array00XXXXXX[64] =
{
	{ NULL, 0},		
	{	Exec_JUMP_FW, 1},			{ Exec_JUMP_FW_W, 1},			{	Exec_JUMP_BW, 1},
	{	Exec_JUMP_BW_W, 1},		{ Exec_TJUMP_FW, 1},			{ Exec_TJUMP_FW_W, 1},			
	{	Exec_TJUMP_BW, 1},		{ Exec_TJUMP_BW_W, 1},		{ Exec_CALL, 1},					
	{	Exec_CALL_LIB, 1},		{ Exec_CALL_LIB_W, 1},		{ Exec_CALL_URL, 1},			
	{	Exec_CALL_URL_W, 1},	{ Exec_LOAD_VAR, 1},			{ Exec_STORE_VAR, 1},			
	{	Exec_INCR_VAR, 1},		{ Exec_DECR_VAR, 1},			{ Exec_LOAD_CONST, 1},		
	{	Exec_LOAD_CONST_W, 1},{ Exec_CONST_0, 1},				{ Exec_CONST_1, 1},				
	{	Exec_CONST_M1, 1},		{ Exec_CONST_ES, 1},			{ Exec_CONST_INVALID, 1},	
	{	Exec_CONST_TRUE, 1},	{ Exec_CONST_FALSE, 1},		{ Exec_INCR, 1},					
	{	Exec_DECR, 1},				{ Exec_ADD_ASG, 1},				{ Exec_SUB_ASG, 1},				
	{	Exec_UMINUS, 1},			{ Exec_ADD, 1},						{ Exec_SUB, 1},						
	{	Exec_MUL, 1},					{ Exec_DIV, 1},						{ Exec_IDIV, 1},					
	{	Exec_REM, 1},					{ Exec_B_AND, 1},					{ Exec_B_OR, 1},					
	{	Exec_B_XOR, 1},				{ Exec_B_NOT, 1},					{ Exec_B_LSHIFT, 1},			
	{	Exec_B_RSSHIFT, 1},		{ Exec_B_RSZSHIFT, 1},		{ Exec_EQ, 1},						
	{	Exec_LE, 1},					{ Exec_LT, 1},						{ Exec_GE, 1},						
	{	Exec_GT, 1},					{ Exec_NE, 1},						{ Exec_NOT, 1},						
	{	Exec_SCAND, 1},				{ Exec_SCOR, 1},					{ Exec_TOBOOL, 1},				
	{	Exec_POP, 1},					{ Exec_TYPEOF, 1},				{ Exec_ISVALID, 1},				
	{	Exec_RETURN, 1},			{ Exec_RETURN_ES, 1},			{ Exec_DEBUG, 1},
	{ NULL, 0},							{ NULL, 0},								{ NULL, 0}
};



enumErrorCode ExecBytecodeInstr( BYTE instr, pstructSI si )
{
	UINT8		arrayIndex;

	if ((instr != 0) && (si != NULL)) 
	{
		if ( (instr & 0x80) == 0x80 ) 
		{
			arrayIndex = (instr & 0x60) >> 5;
			si->currTime += array1XXPPPPP[ arrayIndex ].timeCost;
			if (array1XXPPPPP[ arrayIndex ].func != NULL)
			{
				return (array1XXPPPPP[ arrayIndex ].func)( instr, si );
			}
			else
			{
				return ERR_WAE_WMLS_VERIFICATION;
			}
		}
		else if ( (instr & 0xE0) == 0x40 ) 
		{
			arrayIndex = (instr & 0x10) >> 4;
			si->currTime += array010XPPPP[ arrayIndex ].timeCost;
			if (array010XPPPP[ arrayIndex ].func != NULL)
			{
				return (array010XPPPP[ arrayIndex ].func)( instr, si );
			}
			else
			{
				return ERR_WAE_WMLS_VERIFICATION;
			}
		}
		else if ( (instr & 0xE0) == 0x60 ) 
		{
			arrayIndex = (instr & 0x18) >> 3;
			si->currTime += array011XXPPP[ arrayIndex ].timeCost;
			if (array011XXPPP[ arrayIndex ].func != NULL)
			{
				return (array011XXPPP[ arrayIndex ].func)( instr, si );
			}
			else
			{
				return ERR_WAE_WMLS_VERIFICATION;
			}
		}
		else if ( (instr & 0xC0) == 0x00 ) 
		{
			arrayIndex = instr & 0x3F;
			si->currTime += array00XXXXXX[ arrayIndex ].timeCost;
			if (array00XXXXXX[ arrayIndex ].func != NULL)
			{
				return (array00XXXXXX[ arrayIndex ].func)( si );
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
	return ERR_WAE_WMLS_NULL;
}



/* Control flow instructions ***********************************************************/

enumErrorCode Exec_JUMP_FW_S( BYTE instr, pstructSI si )
{
	UINT8			offset;

	offset = Read5bFrom8b( instr );

	if ( RE_Jump( si->RE, offset ) ) {	
		/* at the given offset from the address of the 
		first byte following this instruction */
		return ERR_WAE_WMLS_NONE;
	}
	else {
		return ERR_WAE_WMLS_VERIFICATION;
	}
}


enumErrorCode Exec_JUMP_FW( pstructSI si ) 
{
	UINT8			offset;

	if (	RE_Read8bOperand( si->RE, &offset ) && 
				RE_Jump( si->RE, offset ) ) {
		/* at the given offset from the address of the 
		first byte following this instruction */
		return ERR_WAE_WMLS_NONE;
	}

	return ERR_WAE_WMLS_VERIFICATION;
}


enumErrorCode Exec_JUMP_FW_W( pstructSI si ) 
{
	UINT16			offset;

	if (	RE_Read16bOperand( si->RE, &offset ) && 
				RE_Jump( si->RE, offset ) ) {
		/* at the given offset from the address of the 
		first byte following this instruction */
		return ERR_WAE_WMLS_NONE;
	}

	return ERR_WAE_WMLS_VERIFICATION;
}


enumErrorCode Exec_JUMP_BW_S( BYTE instr, pstructSI si ) 
{
	UINT8			offset;

	offset = Read5bFrom8b( instr );

	if ( RE_Jump( si->RE, -((INT32)offset)-1 ) ) {	
		/* at the given offset from the address of this instruction */
		return ERR_WAE_WMLS_NONE;
	}
	else {
		return ERR_WAE_WMLS_VERIFICATION;
	}
}


enumErrorCode Exec_JUMP_BW( pstructSI si ) 
{
	UINT8			offset;

	if (	RE_Read8bOperand( si->RE, &offset ) && 
				RE_Jump( si->RE, -((INT32)offset)-2 ) ) {
		/* at the given offset from the address of this instruction */
		return ERR_WAE_WMLS_NONE;
	}

	return ERR_WAE_WMLS_VERIFICATION;
}


enumErrorCode Exec_JUMP_BW_W( pstructSI si ) 
{
	UINT16			offset;

	if (	RE_Read16bOperand( si->RE, &offset ) && 
				RE_Jump( si->RE, -((INT32)offset)-3 ) ) {
		/* at the given offset from the address of this instruction */
		return ERR_WAE_WMLS_NONE;
	}

	return ERR_WAE_WMLS_VERIFICATION;
}


enumErrorCode Exec_TJUMP_FW_S( BYTE instr, pstructSI si ) 
{
	UINT8				offset;
	BOOL				makeJump = TRUE;
	pstructVar	op1;


	offset = Read5bFrom8b( instr );
	op1 = RE_Pop( si->RE );

	if (op1 == NULL)
		return ERR_WAE_WMLS_STACK_UNDERFLOW;

	if (Var_ConvertMethod( CONVERT_BOOLS, op1, NULL )) {
			makeJump = ! (op1->val.theInt);	/* theInt is also the placeholder of the bool value */
			/* only jump on false or invalid */
	}
	/* if convert failed it is treated as invalid which leads to a jump */

	Var_Delete( &op1 );	/* op1 is no longer needed */

	if (makeJump) {

		if ( RE_Jump( si->RE, offset ) ) {	
		/* at the given offset from the address of the 
		first byte following this instruction */
			return ERR_WAE_WMLS_NONE;
		}
		else {
			return ERR_WAE_WMLS_VERIFICATION;
		}

	}
	else {
		return ERR_WAE_WMLS_NONE;
	}
}


enumErrorCode Exec_TJUMP_FW( pstructSI si ) 
{
	UINT8				offset;
	BOOL				makeJump = TRUE;
	pstructVar	op1;


	if ( ! RE_Read8bOperand( si->RE, &offset )) {
		return ERR_WAE_WMLS_VERIFICATION;
	}

	op1 = RE_Pop( si->RE );
	if (op1 == NULL)
		return ERR_WAE_WMLS_STACK_UNDERFLOW;

	if (Var_ConvertMethod( CONVERT_BOOLS, op1, NULL )) {
			makeJump = ! (op1->val.theInt);	/* theInt is also the placeholder of the bool value */
			/* only jump on false or invalid */
	}
	/* if convert failed it is treated as invalid which leads to a jump */

	Var_Delete( &op1 );	/* op1 is no longer needed */

	if (makeJump) {

		if ( RE_Jump( si->RE, offset ) ) {	
		/* at the given offset from the address of the 
		first byte following this instruction */
			return ERR_WAE_WMLS_NONE;
		}
		else {
			return ERR_WAE_WMLS_VERIFICATION;
		}

	}
	else {
		return ERR_WAE_WMLS_NONE;
	}
}


enumErrorCode Exec_TJUMP_FW_W( pstructSI si ) 
{
	UINT16				offset;
	BOOL				makeJump = TRUE;
	pstructVar	op1;


	if ( ! RE_Read16bOperand( si->RE, &offset )) {
		return ERR_WAE_WMLS_VERIFICATION;
	}

	op1 = RE_Pop( si->RE );
	if (op1 == NULL)
		return ERR_WAE_WMLS_STACK_UNDERFLOW;

	if (Var_ConvertMethod( CONVERT_BOOLS, op1, NULL )) {
			makeJump = ! (op1->val.theInt);	/* theInt is also the placeholder of the bool value */
			/* only jump on false or invalid */
	}
	/* if convert failed it is treated as invalid which leads to a jump */

	Var_Delete( &op1 );	/* op1 is no longer needed */

	if (makeJump) {

		if ( RE_Jump( si->RE, offset ) ) {	
		/* at the given offset from the address of the 
		first byte following this instruction */
			return ERR_WAE_WMLS_NONE;
		}
		else {
			return ERR_WAE_WMLS_VERIFICATION;
		}

	}
	else {
		return ERR_WAE_WMLS_NONE;
	}
}


enumErrorCode Exec_TJUMP_BW( pstructSI si ) 
{
	UINT8				offset;
	BOOL				makeJump = TRUE;
	pstructVar	op1;


	if ( ! RE_Read8bOperand( si->RE, &offset )) {
		return ERR_WAE_WMLS_VERIFICATION;
	}

	op1 = RE_Pop( si->RE );
	if (op1 == NULL)
		return ERR_WAE_WMLS_STACK_UNDERFLOW;

	if (Var_ConvertMethod( CONVERT_BOOLS, op1, NULL )) {
			makeJump = ! (op1->val.theInt);	/* theInt is also the placeholder of the bool value */
			/* only jump on false or invalid */
	}
	/* if convert failed it is treated as invalid which leads to a jump */

	Var_Delete( &op1 );	/* op1 is no longer needed */

	if (makeJump) {

		if ( RE_Jump( si->RE, -((INT32)offset)-2 ) ) {	
		/* at the given offset from the address of the 
		first byte following this instruction */
			return ERR_WAE_WMLS_NONE;
		}
		else {
			return ERR_WAE_WMLS_VERIFICATION;
		}

	}
	else {
		return ERR_WAE_WMLS_NONE;
	}
}


enumErrorCode Exec_TJUMP_BW_W( pstructSI si ) 
{
	UINT16			offset;
	BOOL				makeJump = TRUE;
	pstructVar	op1;


	if ( ! RE_Read16bOperand( si->RE, &offset )) {
		return ERR_WAE_WMLS_VERIFICATION;
	}

	op1 = RE_Pop( si->RE );
	if (op1 == NULL) {
		Var_Delete( &op1 );
		return ERR_WAE_WMLS_STACK_UNDERFLOW;
	}

	if (Var_ConvertMethod( CONVERT_BOOLS, op1, NULL )) {
			makeJump = ! (op1->val.theInt);	/* theInt is also the placeholder of the bool value */
			/* only jump on false or invalid */
	}
	/* if convert failed it is treated as invalid which leads to a jump */

	Var_Delete( &op1 );	/* op1 is no longer needed */

	if (makeJump) {

		if ( RE_Jump( si->RE, -((INT32)offset)-3 ) ) {	
		/* at the given offset from the address of the 
		first byte following this instruction */
			return ERR_WAE_WMLS_NONE;
		}
		else {
			return ERR_WAE_WMLS_VERIFICATION;
		}

	}
	else {
		return ERR_WAE_WMLS_NONE;
	}
}



/* Function call instructions ******************************************************************/
enumErrorCode Exec_CALL_S( BYTE instr, pstructSI si )
/*
CORRECTION 990226 (KHN)
=======================
Added so that a local function call makes sure to
report the stack underflow error. This happens if
there are less operands on the stack than the function
has as number of arguments.
*/
{
	UINT8				findex = 0;

	findex = Read3bFrom8b( instr );

	if (findex < si->RE->BPI->nbrOfFunctions) {
		/* valid function index */

		/* set the return IP*/

		FS_Push( si->RE->FS, &(si->RE->currF), si->RE->IP );
		si->RE->currF = Frame_New( findex, si->RE->BPI, si->RE->OpS );

    if (si->RE->currF == NULL)
    {	/* not enough operands on the OpS to match number of function arguments */
    	return ERR_WAE_WMLS_STACK_UNDERFLOW;
    }

		/* set the IP so the next instruction to be executed will be in the new function */
		si->RE->IP = si->RE->BPI->functions[ findex ].startPos;

		return ERR_WAE_WMLS_NONE;
	}
	else {
		return ERR_WAE_WMLS_VERIFICATION;
	}
}


enumErrorCode Exec_CALL( pstructSI si )
/*
CORRECTION 990226 (KHN)
=======================
Added so that a local function call makes sure to
report the stack underflow error. This happens if
there are less operands on the stack than the function
has as number of arguments.
*/
{
	UINT8				findex = 0;

	if ( ! RE_Read8bOperand( si->RE, &findex )) {
		return ERR_WAE_WMLS_VERIFICATION;
	}

	if (findex < si->RE->BPI->nbrOfFunctions) {
		/* valid function index */
		FS_Push( si->RE->FS, &(si->RE->currF), si->RE->IP );
		si->RE->currF = Frame_New( findex, si->RE->BPI, si->RE->OpS );

		/* set the IP so the next instruction to be executed will be in the new function */
		si->RE->IP = si->RE->BPI->functions[ findex ].startPos;

    if (si->RE->currF == NULL)
    {	/* not enough operands on the OpS to match number of function arguments */
    	return ERR_WAE_WMLS_STACK_UNDERFLOW;
    }

		return ERR_WAE_WMLS_NONE;
	}
	else {
		return ERR_WAE_WMLS_VERIFICATION;
	}
}


enumErrorCode Exec_CALL_LIB_S( BYTE instr, pstructSI si ) 
{
	UINT8				findex = 0,
							lindex = 0;

	findex = Read3bFrom8b( instr );

	if ( ! RE_Read8bOperand( si->RE, &lindex )) {
		return ERR_WAE_WMLS_VERIFICATION;
	}
	return CallLibraryFunction( si, lindex, findex );
}


enumErrorCode Exec_CALL_LIB( pstructSI si ) 
{
	UINT8				findex = 0;
	UINT8				lindex = 0;

	if ( ! RE_Read8bOperand( si->RE, &findex )) {
		return ERR_WAE_WMLS_VERIFICATION;
	}
	if ( ! RE_Read8bOperand( si->RE, &lindex )) {
		return ERR_WAE_WMLS_VERIFICATION;
	}
	return CallLibraryFunction( si, lindex, findex );
}


enumErrorCode Exec_CALL_LIB_W( pstructSI si ) 
{
	UINT8				findex = 0;
	UINT16			lindex = 0;

	if ( ! RE_Read8bOperand( si->RE, &findex )) {
		return ERR_WAE_WMLS_VERIFICATION;
	}
	if ( ! RE_Read16bOperand( si->RE, &lindex )) {
		return ERR_WAE_WMLS_VERIFICATION;
	}
	return CallLibraryFunction( si, lindex, findex );
}


enumErrorCode Exec_CALL_URL( pstructSI si )
{
	UINT8						cindexURL		= 0;
	UINT8						cindexFUNC	= 0;
	UINT8						nbrOfArgs		= 0;
	pstructVar			url = NULL;
	WCHAR*					resolvedUrl = NULL;
	BYTE*						utf8_funcName = NULL;
	enumConstType		constType;
	enumErrorCode		error = ERR_WAE_WMLS_VERIFICATION;

	if ( ! RE_Read8bOperand( si->RE, &cindexURL )) {
		return ERR_WAE_WMLS_VERIFICATION;
	}
	if ( ! RE_Read8bOperand( si->RE, &cindexFUNC )) {
		return ERR_WAE_WMLS_VERIFICATION;
	}
	if ( ! RE_Read8bOperand( si->RE, &nbrOfArgs )) {
		return ERR_WAE_WMLS_VERIFICATION;
	}

	if (BPI_GetConstantType( si->RE->BPI, cindexURL, &constType ))
	{
		utf8_funcName = BPI_GetRawUTF8Constant( si->RE->BPI, si->RE->BP, si->RE->BPend, cindexFUNC );
		if ((constType >= CONST_TYPE_UTF8) && (constType <= CONST_TYPE_EXT_CHARSET) && (utf8_funcName != NULL) )
		{
			url = BPI_GetConstant( si->RE->BPI, si->RE->BP, si->RE->BPend, cindexURL );
			if (url != NULL)
			{
				if (w_Resolve (si->baseURL, url->val.theString, &resolvedUrl))
				{
					si->returnInfo = SI_RETURN_EXTERNAL;

					if ( SI_NewReturnParams( si ) )
					{
						/* the SIs returnparams struct is to be filled in */
						si->returnParams->islibCall = FALSE;

						si->returnParams->bpUrl = resolvedUrl;
						resolvedUrl = NULL;

						si->returnParams->funcName = utf8_funcName;
						utf8_funcName = NULL;

           	si->returnParams->argOpS = OpS_NewFromOpS( si->RE->OpS, nbrOfArgs );
						if (si->returnParams->argOpS != NULL)
						{
							error = ERR_WAE_WMLS_NONE;
						}
						else
						{
							error = ERR_WAE_WMLS_STACK_UNDERFLOW;
						}
					}
				}
				/* else ERR_WAE_WMLS_VERIFICATION (the init value of error) */
			}
		}
	}

	DEALLOC( &resolvedUrl );
	Var_Delete( &url );
	DEALLOC( &utf8_funcName );

	return error;
}


enumErrorCode Exec_CALL_URL_W( pstructSI si )
{
	UINT16					cindexURL		= 0;
	UINT16					cindexFUNC	= 0;
	UINT8						nbrOfArgs		= 0;
	pstructVar			url = NULL;
	WCHAR*					resolvedUrl = NULL;
	BYTE*						utf8_funcName = NULL;
	enumConstType		constType;
	enumErrorCode				error = ERR_WAE_WMLS_VERIFICATION;

	if ( ! RE_Read16bOperand( si->RE, &cindexURL )) {
		return ERR_WAE_WMLS_VERIFICATION;
	}
	if ( ! RE_Read16bOperand( si->RE, &cindexFUNC )) {
		return ERR_WAE_WMLS_VERIFICATION;
	}
	if ( ! RE_Read8bOperand( si->RE, &nbrOfArgs )) {
		return ERR_WAE_WMLS_VERIFICATION;
	}

	if (BPI_GetConstantType( si->RE->BPI, cindexURL, &constType ))
	{
		utf8_funcName = BPI_GetRawUTF8Constant( si->RE->BPI, si->RE->BP, si->RE->BPend, cindexFUNC );
		if ((constType >= CONST_TYPE_UTF8) && (constType <= CONST_TYPE_EXT_CHARSET) && (utf8_funcName != NULL) )
		{
			url = BPI_GetConstant( si->RE->BPI, si->RE->BP, si->RE->BPend, cindexURL );
			if (url != NULL)
			{
				if (w_Resolve (si->baseURL, url->val.theString, &resolvedUrl))
				{
					si->returnInfo = SI_RETURN_EXTERNAL;

					if ( SI_NewReturnParams( si ) )
					{
						/* the SIs returnparams struct is to be filled in */
						si->returnParams->islibCall = FALSE;

						si->returnParams->bpUrl = resolvedUrl;
						resolvedUrl = NULL;

						si->returnParams->funcName = utf8_funcName;
						utf8_funcName = NULL;

           	si->returnParams->argOpS = OpS_NewFromOpS( si->RE->OpS, nbrOfArgs );
						if (si->returnParams->argOpS != NULL)
						{
							error = ERR_WAE_WMLS_NONE;
						}
						else
						{
							error = ERR_WAE_WMLS_STACK_UNDERFLOW;
						}
					}
				}
				/* else ERR_WAE_WMLS_VERIFICATION (the init value of error) */
			}
		}
	}

	DEALLOC( &resolvedUrl );
	Var_Delete( &url );
	DEALLOC( &utf8_funcName );

	return error;
}



/* Variable access and manipulation *****************************************************/
enumErrorCode Exec_LOAD_VAR_S( BYTE instr, pstructSI si ) 
{
	UINT8				vindex = 0;
	pstructVar	var;

	vindex = Read5bFrom8b( instr );

	if (vindex <	(si->RE->BPI->functions[ si->RE->currF->funcIndex ].nbrOfArgs +
								si->RE->BPI->functions[ si->RE->currF->funcIndex ].nbrOfVars) ) {
		/* valid variable index */

		var = Var_New();
		if (var == NULL) 
			return ERR_WAE_OUT_OF_MEMORY;

		Var_AssignVar( var, si->RE->currF->variables[ vindex ] );
			/* copies the vindex value to var */
		OpS_Push( si->RE->OpS, &var );

		return ERR_WAE_WMLS_NONE;
	}
	else {
		return ERR_WAE_WMLS_VERIFICATION;
	}
}


enumErrorCode Exec_LOAD_VAR( pstructSI si ) 
{
	UINT8				vindex = 0;
	pstructVar	var;

	if ( ! RE_Read8bOperand( si->RE, &vindex )) {
		return ERR_WAE_WMLS_VERIFICATION;
	}

	if (vindex <	(si->RE->BPI->functions[ si->RE->currF->funcIndex ].nbrOfArgs +
								si->RE->BPI->functions[ si->RE->currF->funcIndex ].nbrOfVars) ) {
		/* valid variable index */

		var = Var_New();
		if (var == NULL) 
			return ERR_WAE_OUT_OF_MEMORY;

		Var_AssignVar( var, si->RE->currF->variables[ vindex ] );
			/* copies the vindex value to var */
		OpS_Push( si->RE->OpS, &var );

		return ERR_WAE_WMLS_NONE;
	}
	else {
		return ERR_WAE_WMLS_VERIFICATION;
	}
}


enumErrorCode Exec_STORE_VAR_S( BYTE instr, pstructSI si ) 
{
	UINT8				vindex = 0;
	pstructVar	var;

	vindex = Read4bFrom8b( instr );

	if (vindex <	(si->RE->BPI->functions[ si->RE->currF->funcIndex ].nbrOfArgs +
								si->RE->BPI->functions[ si->RE->currF->funcIndex ].nbrOfVars) ) {
		/* valid variable index */

		var = RE_Pop( si->RE );
		if (var == NULL) 
			return ERR_WAE_WMLS_STACK_UNDERFLOW;

		Var_AssignVar( si->RE->currF->variables[ vindex ], var );
			/* stores the var value in the correct variable specified by vindex */
		Var_Delete( &var );

		return ERR_WAE_WMLS_NONE;
	}
	else {
		return ERR_WAE_WMLS_VERIFICATION;
	}
}


enumErrorCode Exec_STORE_VAR( pstructSI si ) 
{
	UINT8				vindex = 0;
	pstructVar	var;

	if ( ! RE_Read8bOperand( si->RE, &vindex )) {
		return ERR_WAE_WMLS_VERIFICATION;
	}

	if (vindex <	(si->RE->BPI->functions[ si->RE->currF->funcIndex ].nbrOfArgs +
								si->RE->BPI->functions[ si->RE->currF->funcIndex ].nbrOfVars) ) {
		/* valid variable index */

		var = RE_Pop( si->RE );
		if (var == NULL) 
			return ERR_WAE_WMLS_STACK_UNDERFLOW;

		Var_AssignVar( si->RE->currF->variables[ vindex ], var );
			/* stores the var value in the correct variable specified by vindex */
		Var_Delete( &var );

		return ERR_WAE_WMLS_NONE;
	}
	else {
		return ERR_WAE_WMLS_VERIFICATION;
	}
}


enumErrorCode Exec_INCR_VAR_S( BYTE instr, pstructSI si ) 
{
	UINT8				vindex = 0;

	vindex = Read3bFrom8b( instr );

	if (vindex <	(si->RE->BPI->functions[ si->RE->currF->funcIndex ].nbrOfArgs +
								si->RE->BPI->functions[ si->RE->currF->funcIndex ].nbrOfVars) ) {
		/* valid variable index */

		if ( Var_ConvertMethod(CONVERT_INT_FLOAT, si->RE->currF->variables[ vindex ], NULL) )
    {
			Var_INCR( si->RE->currF->variables[ vindex ] );
    }
    else
    {
			Var_AssignInvalid( si->RE->currF->variables[ vindex ] );
    }

		return ERR_WAE_WMLS_NONE;
	}
	else {
		return ERR_WAE_WMLS_VERIFICATION;
	}
}


enumErrorCode Exec_INCR_VAR( pstructSI si ) 
{
	UINT8				vindex = 0;

	if ( ! RE_Read8bOperand( si->RE, &vindex )) {
		return ERR_WAE_WMLS_VERIFICATION;
	}

	if (vindex <	(si->RE->BPI->functions[ si->RE->currF->funcIndex ].nbrOfArgs +
								si->RE->BPI->functions[ si->RE->currF->funcIndex ].nbrOfVars) ) {
		/* valid variable index */

		if ( Var_ConvertMethod(CONVERT_INT_FLOAT, si->RE->currF->variables[ vindex ], NULL) )
    {
			Var_INCR( si->RE->currF->variables[ vindex ] );
    }
    else
    {
			Var_AssignInvalid( si->RE->currF->variables[ vindex ] );
    }

		return ERR_WAE_WMLS_NONE;
	}
	else {
		return ERR_WAE_WMLS_VERIFICATION;
	}
}


enumErrorCode Exec_DECR_VAR( pstructSI si )
{
	UINT8				vindex = 0;

	if ( ! RE_Read8bOperand( si->RE, &vindex )) {
		return ERR_WAE_WMLS_VERIFICATION;
	}

	if (vindex <	(si->RE->BPI->functions[ si->RE->currF->funcIndex ].nbrOfArgs +
								si->RE->BPI->functions[ si->RE->currF->funcIndex ].nbrOfVars) ) {
		/* valid variable index */

		if ( Var_ConvertMethod(CONVERT_INT_FLOAT, si->RE->currF->variables[ vindex ], NULL) )
    {
			Var_DECR( si->RE->currF->variables[ vindex ] );
    }
    else
    {
			Var_AssignInvalid( si->RE->currF->variables[ vindex ] );
    }

		return ERR_WAE_WMLS_NONE;
	}
	else {
		return ERR_WAE_WMLS_VERIFICATION;
	}
}



/* Access to constants *****************************************************************/
enumErrorCode Exec_LOAD_CONST_S( BYTE instr, pstructSI si )
{
	UINT8				cindex = 0;
	pstructVar	var;

	cindex = Read4bFrom8b( instr );

	if (cindex < si->RE->BPI->nbrOfConstants ) {
		/* valid constant index */

		var = BPI_GetConstant( si->RE->BPI, si->RE->BP, si->RE->BPend, cindex );
		if (var == NULL)
			return ERR_WAE_OUT_OF_MEMORY;

		OpS_Push( si->RE->OpS, &var );

		return ERR_WAE_WMLS_NONE;
	}
	else {
		return ERR_WAE_WMLS_VERIFICATION;
	}
}


enumErrorCode Exec_LOAD_CONST( pstructSI si ) 
{
	UINT8				cindex = 0;
	pstructVar	var;

	if ( ! RE_Read8bOperand( si->RE, &cindex )) {
		return ERR_WAE_WMLS_VERIFICATION;
	}

	if (cindex < si->RE->BPI->nbrOfConstants ) {
		/* valid constant index */

		var = BPI_GetConstant( si->RE->BPI, si->RE->BP, si->RE->BPend, cindex );
		if (var == NULL) 
			return ERR_WAE_OUT_OF_MEMORY;

		OpS_Push( si->RE->OpS, &var );

		return ERR_WAE_WMLS_NONE;
	}
	else {
		return ERR_WAE_WMLS_VERIFICATION;
	}
}


enumErrorCode Exec_LOAD_CONST_W( pstructSI si ) 
{
	UINT16			cindex = 0;
	pstructVar	var;

	if ( ! RE_Read16bOperand( si->RE, &cindex )) {
		return ERR_WAE_WMLS_VERIFICATION;
	}

	if (cindex < si->RE->BPI->nbrOfConstants ) {
		/* valid constant index */

		var = BPI_GetConstant( si->RE->BPI, si->RE->BP, si->RE->BPend, cindex );
		if (var == NULL)
			return ERR_WAE_OUT_OF_MEMORY;

		OpS_Push( si->RE->OpS, &var );

		return ERR_WAE_WMLS_NONE;
	}
	else {
		return ERR_WAE_WMLS_VERIFICATION;
	}
}


enumErrorCode Exec_CONST_0( pstructSI si ) 
{
	pstructVar			var = Var_New();

	Var_AssignInt( var, 0 );
	OpS_Push( si->RE->OpS, &var );

	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Exec_CONST_1( pstructSI si ) 
{
	pstructVar			var = Var_New();

	Var_AssignInt( var, 1 );
	OpS_Push( si->RE->OpS, &var );

	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Exec_CONST_M1( pstructSI si ) 
{
	pstructVar			var = Var_New();

	Var_AssignInt( var, -1 );
	OpS_Push( si->RE->OpS, &var );

	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Exec_CONST_ES( pstructSI si ) 
{
	pstructVar			var = Var_New();
									/* A new var is by default an empty string (ES) */

	OpS_Push( si->RE->OpS, &var );

	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Exec_CONST_INVALID( pstructSI si ) 
{
	pstructVar			var = Var_New();

	Var_AssignInvalid( var );
	OpS_Push( si->RE->OpS, &var );

	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Exec_CONST_TRUE( pstructSI si )
{
	pstructVar			var = Var_New();

	Var_AssignBool( var, TRUE );
	OpS_Push( si->RE->OpS, &var );

	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Exec_CONST_FALSE( pstructSI si ) 
{
	pstructVar			var = Var_New();

	Var_AssignBool( var, FALSE );
	OpS_Push( si->RE->OpS, &var );

	return ERR_WAE_WMLS_NONE;
}


/* Arithmetic instructions **************************************************************/
enumErrorCode Exec_INCR( pstructSI si )
{
	pstructVar		op1 =	RE_Pop( si->RE );

	if (op1 == NULL)
		return ERR_WAE_WMLS_STACK_UNDERFLOW;

	if (Var_ConvertMethod( CONVERT_INT_FLOAT, op1, NULL )) {
		Var_INCR( op1 );
	}
	else {
		Var_AssignInvalid( op1 );
	}
	OpS_Push( si->RE->OpS, &op1 );
	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Exec_DECR( pstructSI si )
{
	pstructVar		op1 =	RE_Pop( si->RE );

	if (op1 == NULL)
		return ERR_WAE_WMLS_STACK_UNDERFLOW;

	if (Var_ConvertMethod( CONVERT_INT_FLOAT, op1, NULL )) {
		Var_DECR( op1 );
	}
	else {
		Var_AssignInvalid( op1 );
	}
	OpS_Push( si->RE->OpS, &op1 );
	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Exec_ADD_ASG( pstructSI si )
{
	UINT8				vindex;
	pstructVar	op1 = NULL;
	pstructVar	op2 = NULL; 
	pstructVar	result = NULL;

	if ( ! RE_Read8bOperand( si->RE, &vindex ))
		return ERR_WAE_WMLS_VERIFICATION;

	if (vindex <	(si->RE->BPI->functions[ si->RE->currF->funcIndex ].nbrOfArgs +
								si->RE->BPI->functions[ si->RE->currF->funcIndex ].nbrOfVars) ) {
		/* valid variable index */

		op1 = si->RE->currF->variables[ vindex ];
		op2 = RE_Pop( si->RE );

		if (op2 == NULL)
				return ERR_WAE_WMLS_STACK_UNDERFLOW;

		if ( Var_ConvertMethod(CONVERT_INTS_FLOATS_STRINGS, op1, op2) ) {
			result = Var_ADD( op1, op2 );
			Var_AssignVar( op1, result );
		}
		else {
			Var_AssignInvalid( op1 );
		}

		Var_Delete( &op2 );
		Var_Delete( &result );

		return ERR_WAE_WMLS_NONE;
	}
	else {
		return ERR_WAE_WMLS_VERIFICATION;
	}
}


enumErrorCode Exec_SUB_ASG( pstructSI si )
{
	UINT8				vindex;
	pstructVar	op1 = NULL;
	pstructVar	op2 = NULL;
	pstructVar	result = NULL;

	if ( ! RE_Read8bOperand( si->RE, &vindex ))
		return ERR_WAE_WMLS_VERIFICATION;

	if (vindex <	(si->RE->BPI->functions[ si->RE->currF->funcIndex ].nbrOfArgs +
								si->RE->BPI->functions[ si->RE->currF->funcIndex ].nbrOfVars) ) {
		/* valid variable index */

		op1 = si->RE->currF->variables[ vindex ];
		op2 = RE_Pop( si->RE );

		if (op2 == NULL)
				return ERR_WAE_WMLS_STACK_UNDERFLOW;

		if ( Var_ConvertMethod(CONVERT_INTS_FLOATS, op1, op2) ) {
			result = Var_SUB( op1, op2 );
			Var_AssignVar( op1, result );
		}
		else {
			Var_AssignInvalid( op1 );
		}

		Var_Delete( &op2 );
		Var_Delete( &result );

		return ERR_WAE_WMLS_NONE;
	}
	else {
		return ERR_WAE_WMLS_VERIFICATION;
	}
}


enumErrorCode Exec_UMINUS( pstructSI si )
{
	pstructVar		op1 =	RE_Pop( si->RE );
	pstructVar		result = NULL;

	if (op1 == NULL)
		return ERR_WAE_WMLS_STACK_UNDERFLOW;

	if (Var_ConvertMethod( CONVERT_INT_FLOAT, op1, NULL )) {
		result = Var_UMINUS( op1 );
		Var_AssignVar( op1, result );
		Var_Delete( &result );
	}
	else {
		Var_AssignInvalid( op1 );
	}
	OpS_Push( si->RE->OpS, &op1 );
	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Exec_ADD( pstructSI si )
{
	pstructVar	op2 = RE_Pop( si->RE );		/* NOTE THE ORDER THE OPERANDS ARE POPPED!!! */
	pstructVar	op1 = RE_Pop( si->RE );
	pstructVar	result = NULL;

	if ((op1 != NULL) && (op2 != NULL)) {
		if ( Var_ConvertMethod(CONVERT_INTS_FLOATS_STRINGS, op1, op2) ) {
			result = Var_ADD( op1, op2 );
		}
		else {
			result = Var_New();
			Var_AssignInvalid( result );
		}
		Var_Delete( &op1 );
		Var_Delete( &op2 );
		OpS_Push( si->RE->OpS, &result );

		return ERR_WAE_WMLS_NONE;
	}
	else {
		Var_Delete( &op1 );
		Var_Delete( &op2 );
		return ERR_WAE_WMLS_STACK_UNDERFLOW;
	}
}


enumErrorCode Exec_SUB( pstructSI si )
{
	pstructVar	op2 = RE_Pop( si->RE );		/* NOTE THE ORDER THE OPERANDS ARE POPPED!!! */
	pstructVar	op1 = RE_Pop( si->RE );
	pstructVar	result = NULL;

	if ((op1 != NULL) && (op2 != NULL)) {
		if ( Var_ConvertMethod(CONVERT_INTS_FLOATS, op1, op2) ) {
			result = Var_SUB( op1, op2 );
		}
		else {
			result = Var_New();
			Var_AssignInvalid( result );
		}
		Var_Delete( &op1 );
		Var_Delete( &op2 );
		OpS_Push( si->RE->OpS, &result );

		return ERR_WAE_WMLS_NONE;
	}
	else {
		Var_Delete( &op1 );
		Var_Delete( &op2 );
		return ERR_WAE_WMLS_STACK_UNDERFLOW;
	}
}


enumErrorCode Exec_MUL( pstructSI si )
{
	pstructVar	op2 = RE_Pop( si->RE );		/* NOTE THE ORDER THE OPERANDS ARE POPPED!!! */
	pstructVar	op1 = RE_Pop( si->RE );
	pstructVar	result = NULL;

	if ((op1 != NULL) && (op2 != NULL)) {
		if ( Var_ConvertMethod(CONVERT_INTS_FLOATS, op1, op2) ) {
			result = Var_MUL( op1, op2 );
		}
		else {
			result = Var_New();
			Var_AssignInvalid( result );
		}
		Var_Delete( &op1 );
		Var_Delete( &op2 );
		OpS_Push( si->RE->OpS, &result );

		return ERR_WAE_WMLS_NONE;
	}
	else {
		Var_Delete( &op1 );
		Var_Delete( &op2 );
		return ERR_WAE_WMLS_STACK_UNDERFLOW;
	}
}


enumErrorCode Exec_DIV( pstructSI si )
{
	pstructVar	op2 = RE_Pop( si->RE );		/* NOTE THE ORDER THE OPERANDS ARE POPPED!!! */
	pstructVar	op1 = RE_Pop( si->RE );
	pstructVar	result = NULL;

	if ((op1 != NULL) && (op2 != NULL)) {
		if ( Var_ConvertMethod(CONVERT_INTS_FLOATS, op1, op2) ) {
			result = Var_DIV( op1, op2 );
		}
		else {
			result = Var_New();
			Var_AssignInvalid( result );
		}
		Var_Delete( &op1 );
		Var_Delete( &op2 );
		OpS_Push( si->RE->OpS, &result );

		return ERR_WAE_WMLS_NONE;
	}
	else {
		Var_Delete( &op1 );
		Var_Delete( &op2 );
		return ERR_WAE_WMLS_STACK_UNDERFLOW;
	}
}


enumErrorCode Exec_IDIV( pstructSI si )
{
	pstructVar	op2 = RE_Pop( si->RE );		/* NOTE THE ORDER THE OPERANDS ARE POPPED!!! */
	pstructVar	op1 = RE_Pop( si->RE );
	pstructVar	result = NULL;

	if ((op1 != NULL) && (op2 != NULL)) {
		if ( Var_ConvertMethod(CONVERT_INTS, op1, op2) ) {
			result = Var_IDIV( op1, op2 );
		}
		else {
			result = Var_New();
			Var_AssignInvalid( result );
		}
		Var_Delete( &op1 );
		Var_Delete( &op2 );
		OpS_Push( si->RE->OpS, &result );

		return ERR_WAE_WMLS_NONE;
	}
	else {
		Var_Delete( &op1 );
		Var_Delete( &op2 );
		return ERR_WAE_WMLS_STACK_UNDERFLOW;
	}
}


enumErrorCode Exec_REM( pstructSI si )
{
	pstructVar	op2 = RE_Pop( si->RE );		/* NOTE THE ORDER THE OPERANDS ARE POPPED!!! */
	pstructVar	op1 = RE_Pop( si->RE );
	pstructVar	result = NULL;

	if ((op1 != NULL) && (op2 != NULL))
  {
		if ( Var_ConvertMethod(CONVERT_INTS, op1, op2) ) /* Change: WAP 1.1 spec changed from  CONVERT_INTS_FLOATS to CONVERT_INTS */
    {
			result = Var_REM( op1, op2 );
		}
		else
    {
			result = Var_New();
			Var_AssignInvalid( result );
		}
		Var_Delete( &op1 );
		Var_Delete( &op2 );
		OpS_Push( si->RE->OpS, &result );

		return ERR_WAE_WMLS_NONE;
	}
	else
  {
		Var_Delete( &op1 );
		Var_Delete( &op2 );
		return ERR_WAE_WMLS_STACK_UNDERFLOW;
	}
}



/* Bitwise instructions **************************************************************************/
enumErrorCode Exec_B_AND( pstructSI si )
{
	pstructVar	op2 = RE_Pop( si->RE );		/* NOTE THE ORDER THE OPERANDS ARE POPPED!!! */
	pstructVar	op1 = RE_Pop( si->RE );
	pstructVar	result = NULL;

	if ((op1 != NULL) && (op2 != NULL)) {
		if ( Var_ConvertMethod(CONVERT_INTS, op1, op2) ) {
			result = Var_B_AND( op1, op2 );
		}
		else {
			result = Var_New();
			Var_AssignInvalid( result );
		}
		Var_Delete( &op1 );
		Var_Delete( &op2 );
		OpS_Push( si->RE->OpS, &result );

		return ERR_WAE_WMLS_NONE;
	}
	else {
		Var_Delete( &op1 );
		Var_Delete( &op2 );
		return ERR_WAE_WMLS_STACK_UNDERFLOW;
	}
}


enumErrorCode Exec_B_OR( pstructSI si )
{
	pstructVar	op2 = RE_Pop( si->RE );		/* NOTE THE ORDER THE OPERANDS ARE POPPED!!! */
	pstructVar	op1 = RE_Pop( si->RE );
	pstructVar	result = NULL;

	if ((op1 != NULL) && (op2 != NULL)) {
		if ( Var_ConvertMethod(CONVERT_INTS, op1, op2) ) {
			result = Var_B_OR( op1, op2 );
		}
		else {
			result = Var_New();
			Var_AssignInvalid( result );
		}
		Var_Delete( &op1 );
		Var_Delete( &op2 );
		OpS_Push( si->RE->OpS, &result );

		return ERR_WAE_WMLS_NONE;
	}
	else {
		Var_Delete( &op1 );
		Var_Delete( &op2 );
		return ERR_WAE_WMLS_STACK_UNDERFLOW;
	}
}


enumErrorCode Exec_B_XOR( pstructSI si )
{
	pstructVar	op2 = RE_Pop( si->RE );		/* NOTE THE ORDER THE OPERANDS ARE POPPED!!! */
	pstructVar	op1 = RE_Pop( si->RE );
	pstructVar	result = NULL;

	if ((op1 != NULL) && (op2 != NULL)) {
		if ( Var_ConvertMethod(CONVERT_INTS, op1, op2) ) {
			result = Var_B_XOR( op1, op2 );
		}
		else {
			result = Var_New();
			Var_AssignInvalid( result );
		}
		Var_Delete( &op1 );
		Var_Delete( &op2 );
		OpS_Push( si->RE->OpS, &result );

		return ERR_WAE_WMLS_NONE;
	}
	else {
		Var_Delete( &op1 );
		Var_Delete( &op2 );
		return ERR_WAE_WMLS_STACK_UNDERFLOW;
	}
}


enumErrorCode Exec_B_NOT( pstructSI si )
{
	pstructVar		op1 =	RE_Pop( si->RE );
	pstructVar		result = NULL;

	if (op1 == NULL)
		return ERR_WAE_WMLS_STACK_UNDERFLOW;

	if (Var_ConvertMethod( CONVERT_INTS, op1, NULL )) {
		result = Var_B_NOT( op1 );
		Var_AssignVar( op1, result );
		Var_Delete( &result );
	}
	else {
		Var_AssignInvalid( op1 );
	}
	OpS_Push( si->RE->OpS, &op1 );
	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Exec_B_LSHIFT( pstructSI si )
{
	pstructVar	op2 = RE_Pop( si->RE );		/* NOTE THE ORDER THE OPERANDS ARE POPPED!!! */
	pstructVar	op1 = RE_Pop( si->RE );
	pstructVar	result = NULL;

	if ((op1 != NULL) && (op2 != NULL)) {
		if ( Var_ConvertMethod(CONVERT_INTS, op1, op2) ) {
			result = Var_B_LSHIFT( op1, op2 );
		}
		else {
			result = Var_New();
			Var_AssignInvalid( result );
		}
		Var_Delete( &op1 );
		Var_Delete( &op2 );
		OpS_Push( si->RE->OpS, &result );

		return ERR_WAE_WMLS_NONE;
	}
	else {
		Var_Delete( &op1 );
		Var_Delete( &op2 );
		return ERR_WAE_WMLS_STACK_UNDERFLOW;
	}
}


enumErrorCode Exec_B_RSSHIFT( pstructSI si )
{
	pstructVar	op2 = RE_Pop( si->RE );		/* NOTE THE ORDER THE OPERANDS ARE POPPED!!! */
	pstructVar	op1 = RE_Pop( si->RE );
	pstructVar	result = NULL;

	if ((op1 != NULL) && (op2 != NULL)) {
		if ( Var_ConvertMethod(CONVERT_INTS, op1, op2) ) {
			result = Var_B_RSSHIFT( op1, op2 );
		}
		else {
			result = Var_New();
			Var_AssignInvalid( result );
		}
		Var_Delete( &op1 );
		Var_Delete( &op2 );
		OpS_Push( si->RE->OpS, &result );

		return ERR_WAE_WMLS_NONE;
	}
	else {
		Var_Delete( &op1 );
		Var_Delete( &op2 );
		return ERR_WAE_WMLS_STACK_UNDERFLOW;
	}
}


enumErrorCode Exec_B_RSZSHIFT( pstructSI si )
{
	pstructVar	op2 = RE_Pop( si->RE );		/* NOTE THE ORDER THE OPERANDS ARE POPPED!!! */
	pstructVar	op1 = RE_Pop( si->RE );
	pstructVar	result = NULL;

	if ((op1 != NULL) && (op2 != NULL)) {
		if ( Var_ConvertMethod(CONVERT_INTS, op1, op2) ) {
			result = Var_B_RSZSHIFT( op1, op2 );
		}
		else {
			result = Var_New();
			Var_AssignInvalid( result );
		}
		Var_Delete( &op1 );
		Var_Delete( &op2 );
		OpS_Push( si->RE->OpS, &result );

		return ERR_WAE_WMLS_NONE;
	}
	else {
		Var_Delete( &op1 );
		Var_Delete( &op2 );
		return ERR_WAE_WMLS_STACK_UNDERFLOW;
	}
}



/* Comparison instructions *****************************************************************/
enumErrorCode Exec_EQ( pstructSI si )
{
	pstructVar	op2 = RE_Pop( si->RE );		/* NOTE THE ORDER THE OPERANDS ARE POPPED!!! */
	pstructVar	op1 = RE_Pop( si->RE );
	pstructVar	result = NULL;

	if ((op1 != NULL) && (op2 != NULL)) {
		if ( Var_ConvertMethod(CONVERT_INTS_FLOATS_STRINGS, op1, op2) ) {
			result = Var_EQ( op1, op2 );
		}
		else {
			result = Var_New();
			Var_AssignInvalid( result );
		}
		Var_Delete( &op1 );
		Var_Delete( &op2 );
		OpS_Push( si->RE->OpS, &result );

		return ERR_WAE_WMLS_NONE;
	}
	else {
		Var_Delete( &op1 );
		Var_Delete( &op2 );
		return ERR_WAE_WMLS_STACK_UNDERFLOW;
	}
}


enumErrorCode Exec_LE( pstructSI si )
{
	pstructVar	op2 = RE_Pop( si->RE );		/* NOTE THE ORDER THE OPERANDS ARE POPPED!!! */
	pstructVar	op1 = RE_Pop( si->RE );
	pstructVar	result = NULL;

	if ((op1 != NULL) && (op2 != NULL)) {
		if ( Var_ConvertMethod(CONVERT_INTS_FLOATS_STRINGS, op1, op2) ) {
			result = Var_LE( op1, op2 );
		}
		else {
			result = Var_New();
			Var_AssignInvalid( result );
		}
		Var_Delete( &op1 );
		Var_Delete( &op2 );
		OpS_Push( si->RE->OpS, &result );

		return ERR_WAE_WMLS_NONE;
	}
	else {
		Var_Delete( &op1 );
		Var_Delete( &op2 );
		return ERR_WAE_WMLS_STACK_UNDERFLOW;
	}
}


enumErrorCode Exec_LT( pstructSI si )
{
	pstructVar	op2 = RE_Pop( si->RE );		/* NOTE THE ORDER THE OPERANDS ARE POPPED!!! */
	pstructVar	op1 = RE_Pop( si->RE );
	pstructVar	result = NULL;

	if ((op1 != NULL) && (op2 != NULL)) {
		if ( Var_ConvertMethod(CONVERT_INTS_FLOATS_STRINGS, op1, op2) ) {
			result = Var_LT( op1, op2 );
		}
		else {
			result = Var_New();
			Var_AssignInvalid( result );
		}
		Var_Delete( &op1 );
		Var_Delete( &op2 );
		OpS_Push( si->RE->OpS, &result );

		return ERR_WAE_WMLS_NONE;
	}
	else {
		Var_Delete( &op1 );
		Var_Delete( &op2 );
		return ERR_WAE_WMLS_STACK_UNDERFLOW;
	}
}


enumErrorCode Exec_GE( pstructSI si )
{
	pstructVar	op2 = RE_Pop( si->RE );		/* NOTE THE ORDER THE OPERANDS ARE POPPED!!! */
	pstructVar	op1 = RE_Pop( si->RE );
	pstructVar	result = NULL;

	if ((op1 != NULL) && (op2 != NULL)) {
		if ( Var_ConvertMethod(CONVERT_INTS_FLOATS_STRINGS, op1, op2) ) {
			result = Var_GE( op1, op2 );
		}
		else {
			result = Var_New();
			Var_AssignInvalid( result );
		}
		Var_Delete( &op1 );
		Var_Delete( &op2 );
		OpS_Push( si->RE->OpS, &result );

		return ERR_WAE_WMLS_NONE;
	}
	else {
		Var_Delete( &op1 );
		Var_Delete( &op2 );
		return ERR_WAE_WMLS_STACK_UNDERFLOW;
	}
}


enumErrorCode Exec_GT( pstructSI si )
{
	pstructVar	op2 = RE_Pop( si->RE );		/* NOTE THE ORDER THE OPERANDS ARE POPPED!!! */
	pstructVar	op1 = RE_Pop( si->RE );
	pstructVar	result = NULL;

	if ((op1 != NULL) && (op2 != NULL)) {
		if ( Var_ConvertMethod(CONVERT_INTS_FLOATS_STRINGS, op1, op2) ) {
			result = Var_GT( op1, op2 );
		}
		else {
			result = Var_New();
			Var_AssignInvalid( result );
		}
		Var_Delete( &op1 );
		Var_Delete( &op2 );
		OpS_Push( si->RE->OpS, &result );

		return ERR_WAE_WMLS_NONE;
	}
	else {
		Var_Delete( &op1 );
		Var_Delete( &op2 );
		return ERR_WAE_WMLS_STACK_UNDERFLOW;
	}
}


enumErrorCode Exec_NE( pstructSI si )
{
	if ((Exec_EQ(si)  == ERR_WAE_WMLS_NONE) && 
			(Exec_NOT(si) == ERR_WAE_WMLS_NONE)) {
		return ERR_WAE_WMLS_NONE;
	}
	else {
		return ERR_WAE_WMLS_STACK_UNDERFLOW;
		/* the only fatal error from EQ and NOT is stack underflow */
	}
}



/* Logical instructions ***********************************************************************/
enumErrorCode Exec_NOT( pstructSI si )
{
	pstructVar	var = RE_Pop( si->RE );

	if (var == NULL) {
		return ERR_WAE_WMLS_STACK_UNDERFLOW;
	}

	if ( VCR_OK == Var_Convert( var, typeBool )) {
		if (var->val.theInt == FALSE) {
			Var_AssignBool( var, TRUE );
		}
		else {
			Var_AssignBool( var, FALSE );
		}
	}
	else {
		Var_AssignInvalid( var );
	}

	OpS_Push( si->RE->OpS, &var );
	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Exec_SCAND( pstructSI si )
{
	pstructVar	var = RE_Pop( si->RE );
	pstructVar	var2 = NULL;


	if (var == NULL) {
		return ERR_WAE_WMLS_STACK_UNDERFLOW;
	}

	if ( VCR_OK == Var_Convert( var, typeBool )) {
		if (var->val.theInt == FALSE) {
			var2 = Var_New();
			Var_AssignBool( var2, FALSE );
		}
	}
	else {
		var2 = Var_New();
		Var_AssignBool( var2, FALSE );
		Var_AssignInvalid( var );
	}

	OpS_Push( si->RE->OpS, &var );
	if (var2 != NULL) {
		OpS_Push( si->RE->OpS, &var2 );
	}
	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Exec_SCOR( pstructSI si )
{
	pstructVar	var = RE_Pop( si->RE );
	pstructVar	var2 = NULL;


	if (var == NULL) {
		return ERR_WAE_WMLS_STACK_UNDERFLOW;
	}

	if ( VCR_OK == Var_Convert( var, typeBool )) {
		if (var->val.theInt == FALSE) {
			Var_AssignBool( var, TRUE );
		}
		else {
			var2 = Var_New();
			Var_AssignBool( var2, FALSE );
		}
	}
	else {
		var2 = Var_New();
		Var_AssignBool( var2, FALSE );
		Var_AssignInvalid( var );
	}

	OpS_Push( si->RE->OpS, &var );
	if (var2 != NULL) {
		OpS_Push( si->RE->OpS, &var2 );
	}
	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Exec_TOBOOL( pstructSI si )
{
	pstructVar	var = RE_Pop( si->RE );

	if (var == NULL) {
		return ERR_WAE_WMLS_STACK_UNDERFLOW;
	}

	if ( ! (VCR_OK == Var_Convert( var, typeBool ))) {
		Var_AssignInvalid( var );
	}

	OpS_Push( si->RE->OpS, &var );
	return ERR_WAE_WMLS_NONE;
}



/* Stack instructions *****************************************************************************/
enumErrorCode Exec_POP( pstructSI si ) 
{
	pstructVar	var = RE_Pop( si->RE );

	if (var == NULL) {
		return ERR_WAE_WMLS_STACK_UNDERFLOW;
	}
	else {
		Var_Delete( &var );
		return ERR_WAE_WMLS_NONE;
	}
}



/* Access to operand type ************************************************************************/
enumErrorCode Exec_TYPEOF( pstructSI si ) 
{
	pstructVar			var = RE_Pop( si->RE );
	pstructVar			type;

	if (var == NULL) 
		return ERR_WAE_WMLS_STACK_UNDERFLOW;

	type = Var_New();
	Var_AssignInt( type, var->type );

	Var_Delete( &var );

	OpS_Push( si->RE->OpS, &type );

	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Exec_ISVALID( pstructSI si ) 
{
	pstructVar			var = RE_Pop( si->RE );
	pstructVar			isValid;

	if (var == NULL) 
		return ERR_WAE_WMLS_STACK_UNDERFLOW;

	isValid = Var_New();
	if (var->type  != typeInvalid) {
		Var_AssignBool( isValid, TRUE );
	}
	else {
		Var_AssignBool( isValid, FALSE );
	}

	Var_Delete( &var );

	OpS_Push( si->RE->OpS, &isValid );

	return ERR_WAE_WMLS_NONE;
}



/* Function return instructions ***************************************************************/
enumErrorCode Exec_RETURN( pstructSI si ) 
{
	UINT16					topIndex = OpS_GetTopIndex( si->RE->OpS );
	pstructVar			funcResult = NULL;
	pstructFrame		returnFrame = si->RE->currF;
	
	/* first make sure that the result is correctly placed on the stack */
	
	if (topIndex != (si->RE->currF->returnSP + 1)) {
		/* there is some scrap on the stack which must be disposed */
		funcResult = RE_Pop( si->RE );
		if ( ! OpS_ClearUntilIndex( si->RE->OpS, si->RE->currF->returnSP ) ) {
			return ERR_WAE_WMLS_STACK_UNDERFLOW;
		}
		OpS_Push( si->RE->OpS, &funcResult );
	}

	si->RE->currF = FS_Pop( si->RE->FS );

	/* check if this was the last function to finish in this BP */
	if( si->RE->currF != NULL ) {
		/* not the last: restore the frame that called */
		si->RE->IP = si->RE->currF->returnIP;
	}
	else {
		/* the last: signal to to SI that this BP is done */
		si->returnInfo = SI_RETURN_DONE;
	}
	Frame_Delete( &returnFrame, si->RE->BPI );

	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Exec_RETURN_ES( pstructSI si ) 
{
	pstructVar			esVar = Var_New();
	pstructFrame		returnFrame = si->RE->currF;
	
	/* first make sure that the result is correctly placed on the stack */
	
	if ( ! OpS_ClearUntilIndex( si->RE->OpS, si->RE->currF->returnSP ) ) {
		return ERR_WAE_WMLS_STACK_UNDERFLOW;
	}
	OpS_Push( si->RE->OpS, &esVar );

	si->RE->currF = FS_Pop( si->RE->FS );

	/* check if this was the last function to finish in this BP */
	if( si->RE->currF != NULL ) {
		/* not the last: restore the frame that called */
		si->RE->IP = si->RE->currF->returnIP;
	}
	else {
		/* the last: signal to to SI that this BP is done */
		si->returnInfo = SI_RETURN_DONE;
	}
	Frame_Delete( &returnFrame, si->RE->BPI );

	return ERR_WAE_WMLS_NONE;
}



/* Miscellaneous instructions *******************************************************************/
enumErrorCode Exec_DEBUG( pstructSI si ) 
{
	si=si; /* just to get rid of a compiler warning */

	return ERR_WAE_WMLS_NONE;
}


