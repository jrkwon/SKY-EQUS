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
#include "si_frame.h"


pstructFrame Frame_New( UINT8 funcIndex, pstructBPInfo BPI, pstructOpS OpS )
{
	pstructFrame			result = NULL;
	UINT16						nbrOfArgs;
	UINT16						nbrOfVars;
	INT16							i;
	UINT16						returnSP = OpS_GetTopIndex( OpS );
	
	if (funcIndex < BPI->nbrOfFunctions) {
		/* a valid function index */
		result = NEWSTRUCT( structFrame );
		result->funcIndex = funcIndex;
		result->returnIP = 0; /* will be correctly set when being pushed on the FS */
		if (BPI->functions[ funcIndex ].nbrOfArgs > returnSP)
		{
			return NULL;
		}
		result->returnSP = (UINT16)( returnSP - (BPI->functions[ funcIndex ].nbrOfArgs) ); 
			/* The arguments to this function frame and thus the return place on the stack have to below the args */
		
		/* init the function variables (args + loc.vars) */
		nbrOfArgs = BPI->functions[ funcIndex ].nbrOfArgs;
		nbrOfVars = nbrOfArgs + BPI->functions[ funcIndex ].nbrOfVars;
		result->variables = NEWARRAY( pstructVar, nbrOfVars );

		/* init the arguments (pop from the OpS and store them in the function frame) */
		for (i=nbrOfArgs - 1; i>=0; i--) {
			result->variables[i] = OpS_Pop( OpS );
		}
		
		/* creates the local vars placeholders (these are initialised to empty strings) */
		for (i=nbrOfArgs; i<nbrOfVars; i++) {
			result->variables[i] = Var_New();
		}
	}
	
	return result;
}


VOID Frame_Delete( pstructFrame *pThis, pstructBPInfo BPI )
{
	UINT8							funcIndex;
	UINT16						nbrOfVars;
	UINT16						i;

	if ((*pThis) != NULL) {
		funcIndex = (*pThis)->funcIndex;
		nbrOfVars = BPI->functions[ funcIndex ].nbrOfArgs + BPI->functions[ funcIndex ].nbrOfVars;

		for (i=0; i<nbrOfVars; i++) {
			Var_Delete( &((*pThis)->variables[i]) );
		}
		DEALLOC( &((*pThis)->variables) );
		DEALLOC( pThis );
	}
}
