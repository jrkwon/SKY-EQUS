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
#include "si_wtai.h"

#include "si_var.h"
#include "si_types.h"
#include "si_ops.h"
#include "si_libs.h"

#include "aapiwta.h"

#if defined LOG_EXTERNAL
	#include "aapiclnt.h"
	#include "logcodes.h"
#endif

#define WTAI_LIB_FUNC_MAX   1

const structLibCall			arrayScriptWTAILib[1][2] =
{
	{ {Call_WTAPublic_makeCall, 1, 1},		{Call_WTAPublic_sendDTMF, 1, 1} }
};



enumErrorCode WTAI_CallLibraryFunction( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
	structLibCall*		theLibEntry = NULL;

	if (funcIndex <= WTAI_LIB_FUNC_MAX)
	{
		theLibEntry = (structLibCall*) &( arrayScriptWTAILib[libIndex - WTAI_LIB_INDEX_START][funcIndex] );
		si->currTime += theLibEntry->timeCost;

		if ((OpS_GetTopIndex( si->RE->OpS ) - theLibEntry->nbrOfArgs) < (si->RE->currF->returnSP))
		{
			return ERR_WAE_WMLS_STACK_UNDERFLOW;
		}

		if (theLibEntry->func != NULL)
		{
			return (theLibEntry->func)( si, libIndex, funcIndex );
		}
	}


	/* There is no standard library function corresponding
  to the libIndex and funcIndex.
  This results in an "invalid" return value */
  /* NOTE!!!!!!!!!!!!!!!
  Deviation from the spec:
  My interpretation of the WMLS Libs spec is that
  a call to a library function that does not exist (no implementation)
  causes a fatal error (ERR_WAE_WMLS_LIB).
  I have found it reasonable to not be so harch and instead give invalid
  as a result when calling such a function. My opinion is that this leads to a
  more robust and failsafe implementation. Especially since some library
  functions are optional amd should this really cause a fatal error.
  !!!!!!!!!!!!!!!!!!! */
	{
    pstructVar		invalidResult = Var_New();

    Var_AssignInvalid( invalidResult );
		OpS_Push( si->RE->OpS, &invalidResult );
		return ERR_WAE_WMLS_NONE;
  }
}


/* Public WTA *************************************************************/

enumErrorCode Call_WTAPublic_makeCall( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
	pstructVar			number = RE_Pop( si->RE );
	pstructVar			result = Var_New();
  BYTE						*pbStr = NULL;
  BOOL						fOverflow = FALSE;

	libIndex=libIndex; /* just to get rid of a compiler warning */
	funcIndex=funcIndex; /* just to get rid of a compiler warning */

	Var_AssignInvalid( result );
  /* invalid will be the result if the function fails */

	if (Var_ConvertMethod( CONVERT_STRINGS, number, NULL ))
  {
   	pbStr = wip_wchar2byte( number->val.theString, &fOverflow );
    if (! fOverflow)
    {
#if defined LOG_EXTERNAL
			CLNTa_log(
				0, log_wae_wmls_publicWTAI,
				"\toperation: %u\n\targument: %s\n",
        0, (CHAR*)pbStr );
#endif
			WTAIa_publicMakeCall( (CHAR*)pbStr );
      Var_NewString( result, 0 );
      /* result is an empty string which means success*/
    }
	}
	OpS_Push( si->RE->OpS, &result );

	Var_Delete( &number );
	Var_Delete( &result );
  DEALLOC( &pbStr );

	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Call_WTAPublic_sendDTMF( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
	pstructVar			number = RE_Pop( si->RE );
	pstructVar			result = Var_New();
  BYTE						*pbStr = NULL;
  BOOL						fOverflow = FALSE;

	libIndex=libIndex; /* just to get rid of a compiler warning */
	funcIndex=funcIndex; /* just to get rid of a compiler warning */

	Var_AssignInvalid( result );
  /* invalid will be the result if the function fails */

	if (Var_ConvertMethod( CONVERT_STRINGS, number, NULL ))
  {
   	pbStr = wip_wchar2byte( number->val.theString, &fOverflow );
    if (! fOverflow)
    {
#if defined LOG_EXTERNAL
			CLNTa_log(
				0, log_wae_wmls_publicWTAI,
				"\toperation: %u\n\targument: %s\n",
        0, (CHAR*)pbStr );
#endif
			WTAIa_publicSendDTMF( (CHAR*)pbStr );
      Var_NewString( result, 0 );
      /* result is an empty string which means success*/
    }
	}
	OpS_Push( si->RE->OpS, &result );

	Var_Delete( &number );
	Var_Delete( &result );
  DEALLOC( &pbStr );

	return ERR_WAE_WMLS_NONE;
}

