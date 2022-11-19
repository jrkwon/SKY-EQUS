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
990614 (KHN) Trouble report 113
								Changes to Float.pow and sqrt were done to catch overflow and
                underflow situations. When making these changes defects were
                introduced which entailed that the correct result was never
                returned to the stack. This has now been resolved.
                (functions: Call_Float_pow and Call_Float_sqrt)

990623 (KHN) Some functions used Float.int but did not make sure the conversion
								succeded. This has now been corrected.
                (Functions:
                Call_Lang_random,
                Call_Lang_seed,
                Call_String_charAt,
                Call_String_subString,
                Call_String_elementAt,
                Call_String_removeAt,
                Call_String_replaceAt,
                Call_String_insertAt )


990624 (KHN) Adding float overflow detection in Float.pow and sqrt have
                introduced more errors. This time a case which should
                give an invalid or special value occured but then a
                secondary check set the value a second time overwriting the
                correct result. Now corrected.

990804 (KHN) Float.int, ceil, floor, round all check so that the float is not
                larger than can be stored in an INT32. Because of bad float
                precision with so large numbers the comparison let through
                too big floats. This has now been corrected.
                Also a problem with Flaot.round on the OSE platform has made
                us rewrite the code to get rid of the use of FLOAT32_EPSILON
                when performing the round. This should now be correct on all
                platforms.

990824 (KHN) WMLBrowser.setVar needs a validation of the value to set the
                variable to. It must be legal XML CData.
                (Changed Call_Browser_setVar and
                added function Setvar_checkSyntax)

990913 (FAR) Call_Browser_newContext modified 

991207 (KHN) WAP 1.2 updates:
                 SEC-18-Jun-1999-7 and 8: String.find and String.replace behavior.

991220 (KHN) A change due to WTA:
                 One must check that the UA is a WTA UA otherwise the result of 
								 the WTA call will be invalid.

000124 (KHN) Implemented the optional lib func Crypto.signText.
                 It is optional and is only to be existing when CAN_SIGN_TEXT is defined.

000215 (KHN) Bugfix in Call_String_subString.
                Adding two integers could result in overflow that
                caused incorrect behaviour.
                Fixed by doing a MAX operation that will correct the overflow

000221 (KHN) Bugfix in the arrayScriptLib constant:
                Fixed a missing parameter for the Call_Lang_parseInt entry.

000308 (KHN) Bugfix in the Lib_textSigned and Call_Crypto_signText functions:
                Fixed calling functions with parameters of wrong types.

000308 (KHN) Replaced the usage of the following macros:
                WTAI_NETWORK_COMMON
                WTAI_GSM
                WTAI_PDC
                WTAI_IS_136 
								with CONFIG_WTA

000802 (KHN) Changed so that the URL.getReferer function includes the fragment
                in its minimum relative path operation.

000802 (KHN) In function String.format, changed type of variable lastmatchPos to INT32.

000808 (KHN) WAP 1.2.1 update:
                CR WMLSL-IBM-20000320-SeedSequence
                Seed independant sequence if the seed is a negative integer.

000929 (KHN) GWC API change for Public WTAI:
                CAlling Public WTAI functions are now a adaptor-connector pair.
								This is from now on handled by WTAI_IF in the same way that
								non-public WTAI calls are handled. This means that all
								code for WMLS (both C and SDL) is the same whatever the 
								configuration. The files si_wtai.c and si_wtai.h are no longer
								needed.
001103 (HEAD) Added support for proprietary script library functions.
                This enables the GWC customer to add handling
								of their own synchronous or asynchronous
								library functions.
								The API for this is provided through an addition
								in the CLNT connector and adaptor API.
								This functionality is only available if
								USE_PROPRIETARY_WMLS_LIBS is defined.
**********/



#include "si_libs.h"

#include "si_var.h"
#include "si_types.h"
#include "si_misc.h"
#ifdef HAS_FLOAT
    #include "si_float.h"
#endif
#include "si_int.h"
#include "url.h"
#include "wmlif.h"
	/* for the parser functions */
#include "tapimmi.h"
	/* for the WTA_UA define */
#include "trnscode.h"
#include "ansilibs.h"

#ifdef CAN_SIGN_TEXT
	#include "si_crpto.h"
	#include "aapimmi.h"
#endif

#ifdef USE_WIP_MALLOC
	#include "wip_mem.h"
#endif


/* CLNTa_currentTime used by Lang.seed */
#include "aapiclnt.h"

#if     defined NO_GLOBAL_VARS
#include "userdata.h"
#endif

#include "wtai_if.h"

#ifdef FEATURE_WAP
BOOL isMusicbellDownload = FALSE;
BOOL attributeInvalid = FALSE;

#define RESULT_STRING_MAX   1500

/* buffer for conversion char to wide char */
WCHAR w_result_buffer[RESULT_STRING_MAX] = {0};

/* buffer for storing extra library function result */
char result_buffer[RESULT_STRING_MAX] = {0};

/* buffer for storing KSC5601 code to be escaped */
/* 693bytes * 2 bytes */
char escape_buffer[RESULT_STRING_MAX*2] = {0};

#endif //#ifdef FEATURE_WAP


#define STD_LIB_MAX         5
#define STD_LIB_FUNC_MAX   15


const structLibCall			arrayScriptLib[6][16] =
{
	{ {Call_Lang_abs, 1, 1},					{Call_Lang_min, 1, 2},
		{Call_Lang_max, 1, 2},					{Call_Lang_parseInt, 1, 1 },
		{Call_Lang_parseFloat, 1, 1},		{Call_Lang_isInt, 1, 1},
		{Call_Lang_isFloat, 1, 1},			{Call_Lang_maxInt, 1, 0},
		{Call_Lang_minInt, 1, 0},				{Call_Lang_Float, 1, 0},
		{Call_Lang_exit, 1, 1},					{Call_Lang_abort, 1, 1},
		{Call_Lang_random, 1, 1},				{Call_Lang_seed, 1, 1},
		{Call_Lang_characterSet, 1, 0},	{NULL, 0, 0} },

	{ {Call_Float_int, 1, 1},					{Call_Float_floor, 1, 1},
		{Call_Float_ceil, 1, 1},				{Call_Float_pow, 1, 2},
		{Call_Float_round, 1, 1},				{Call_Float_sqrt, 1, 1},
		{Call_Float_maxFloat, 1, 0},		{Call_Float_minFloat, 1, 0},
		{NULL, 0, 0},										{NULL, 0, 0},
		{NULL, 0, 0},										{NULL, 0, 0},
		{NULL, 0, 0},										{NULL, 0, 0},
		{NULL, 0, 0},										{NULL, 0, 0} },

	{ {Call_String_length, 1, 1},			{Call_String_isEmpty, 1, 1},
		{Call_String_charAt, 1, 2},			{Call_String_subString, 1, 3},
		{Call_String_find, 1, 2},				{Call_String_replace, 1, 3},
		{Call_String_elements, 1, 2},		{Call_String_elementAt, 1, 3},
		{Call_String_removeAt, 1, 3},		{Call_String_replaceAt, 1, 4},
		{Call_String_insertAt, 1, 4},		{Call_String_squeeze, 1, 1},
		{Call_String_trim, 1, 1},				{Call_String_compare, 1, 2},
		{Call_String_toString, 1, 1},		{Call_String_format, 1, 2} },

	{ {Call_URL_isValid, 1, 1},				{Call_URL_getScheme, 1, 1},
		{Call_URL_getHost, 1, 1},				{Call_URL_getPort, 1, 1},
		{Call_URL_getPath, 1, 1},				{Call_URL_getParameters, 1, 1},
		{Call_URL_getQuery, 1, 1},			{Call_URL_getFragment, 1, 1},
		{Call_URL_getBase, 1, 0},				{Call_URL_getReferer, 1, 0},
		{Call_URL_resolve, 1, 2},				{Call_URL_escapeString, 1, 1},
		{Call_URL_unescapeString, 1, 1},{Call_URL_loadString, 1, 2},
    {NULL, 0, 0},										{NULL, 0, 0} },

	{ {Call_Browser_getVar, 1, 1 },			{Call_Browser_setVar, 1, 2 },
		{Call_Browser_go, 1, 1 },					{Call_Browser_prev, 1, 0 }, 
		{Call_Browser_newContext, 1, 0 }, {Call_Browser_getCurrentCard, 1, 0 }, 
		{Call_Browser_refresh, 1, 0 },		{NULL, 0, 0}, 
		{NULL, 0, 0},										{NULL, 0, 0},												
		{NULL, 0, 0},										{NULL, 0, 0}, 
		{NULL, 0, 0},										{NULL, 0, 0},												
		{NULL, 0, 0},										{NULL, 0, 0} },

	{ {Call_Dialogs_prompt, 1, 2},		{Call_Dialogs_confirm, 1, 3},	
		{Call_Dialogs_alert, 1, 1},			{NULL, 0, 0}, 
		{NULL, 0, 0},										{NULL, 0, 0},									
		{NULL, 0, 0},										{NULL, 0, 0}, 
		{NULL, 0, 0},										{NULL, 0, 0},									
		{NULL, 0, 0},										{NULL, 0, 0},
		{NULL, 0, 0},										{NULL, 0, 0},
		{NULL, 0, 0},										{NULL, 0, 0} }
};

#ifdef FEATURE_WAP

#ifdef FEATURE_GVM

#define retURL_STRING_MAX			100
#define svrIP_STRING_MAX			100
#define dlsIP_STRING_MAX	        100
#define uid_STRING_MAX		        100

/* buffers for storing input argument for extra library functions */
char retURL_buffer[retURL_STRING_MAX] = {0};
char svrIP_buffer[svrIP_STRING_MAX] = {0};
char dlsIP_buffer[dlsIP_STRING_MAX] = {0};
char uid_buffer[uid_STRING_MAX] = {0};

enumErrorCode Call_Netgame_Gamerun( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
	pstructVar	retURL  = RE_Pop( si->RE );
	pstructVar	svrPort = RE_Pop( si->RE );
    pstructVar	svrIP   = RE_Pop( si->RE );
	pstructVar	dlsPort = RE_Pop( si->RE );
	pstructVar	dlsIP   = RE_Pop( si->RE );
    pstructVar	gid     = RE_Pop( si->RE );
	pstructVar	cpid    = RE_Pop( si->RE );
	pstructVar	uid     = RE_Pop( si->RE );
	pstructVar	result  = Var_New();

	libIndex=libIndex; /* just to get rid of a compiler warning */
	funcIndex=funcIndex; /* just to get rid of a compiler warning */

	if ((VCR_OK == Var_Convert( retURL,  typeString )) && 
		(VCR_OK == Var_Convert( svrPort, typeInteger )) &&
		(VCR_OK == Var_Convert( svrIP,   typeString  )) &&
		(VCR_OK == Var_Convert( dlsPort, typeInteger )) &&
		(VCR_OK == Var_Convert( dlsIP,   typeString  )) &&
		(VCR_OK == Var_Convert( gid,     typeInteger )) &&
		(VCR_OK == Var_Convert( cpid,    typeInteger )) &&
		(VCR_OK == Var_Convert( uid,     typeString  ))) {
		 
		/* svrIP_buffer is used for conversion from wchar titleString to char */
		Uni2KSCString(retURL->val.theString, (unsigned char *)retURL_buffer);
		Uni2KSCString(svrIP->val.theString,  (unsigned char *)svrIP_buffer);
		Uni2KSCString(dlsIP->val.theString,  (unsigned char *)dlsIP_buffer);
		Uni2KSCString(uid->val.theString,    (unsigned char *)uid_buffer);

	    CLNTa_log(1,0,"++Netgame_Gamerun is called++\n");

		GoToGVM(uid_buffer,   cpid->val.theInt, gid->val.theInt,
		                dlsIP_buffer, dlsPort->val.theInt, 
						svrIP_buffer, svrPort->val.theInt,
						retURL_buffer,
						result_buffer); 

	    KSC2UniString((unsigned char*)result_buffer, w_result_buffer);

	    Var_AssignString(result, w_strlen(w_result_buffer), w_result_buffer);	

	} else 
	{
		Var_AssignInvalid( result );
	}

	Var_Delete( &retURL );
	Var_Delete( &svrPort);
	Var_Delete( &svrIP  );
	Var_Delete( &dlsPort);
	Var_Delete( &dlsIP  );
	Var_Delete( &gid    );
	Var_Delete( &cpid   );
	Var_Delete( &uid    );

	OpS_Push( si->RE->OpS, &result );
	CLNTa_log(1,0,"++Call_Netgame_Gamerun is complete++\n");

	return ERR_WAE_WMLS_NONE;
}
#endif//#ifdef FEATURE_GVM

enumErrorCode Terminal_isModifyAll( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
	extern void isModifyAll(int Start, int End, char *result);
	pstructVar	entryIDEnd = RE_Pop( si->RE );
	pstructVar	entryIDStart = RE_Pop( si->RE );
	pstructVar	result = Var_New();

	libIndex=libIndex; /* just to get rid of a compiler warning */
	funcIndex=funcIndex; /* just to get rid of a compiler warning */

	if ((VCR_OK == Var_Convert( entryIDEnd, typeInteger )) &&
	    (VCR_OK == Var_Convert( entryIDStart, typeInteger ))) 
	{
		isModifyAll(entryIDStart->val.theInt, entryIDEnd->val.theInt, result_buffer);

		KSC2UniString((unsigned char*)result_buffer, w_result_buffer);

		Var_AssignString(result, w_strlen(w_result_buffer), w_result_buffer);

	} else {
		Var_AssignInvalid( result );
	}

	Var_Delete( &entryIDEnd );
	Var_Delete( &entryIDStart );
	
	OpS_Push( si->RE->OpS, &result );

	return ERR_WAE_WMLS_NONE;
}

#endif //FEATURE_WAP

#ifdef CAN_SIGN_TEXT
	#define LIB_CRYPTO 6
	#define FUNC_CRYPTO_SIGNTEXT 16
	#define FUNC_CRYPTO_SIGNTEXT__timeCost 1
	#define FUNC_CRYPTO_SIGNTEXT__nbrOfArgs 4
#endif



enumErrorCode CallLibraryFunction( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
	structLibCall*		theLibEntry = NULL;

	if (si == NULL)
	{
		return ERR_WAE_WMLS_LIB;
	}

	if ((libIndex <= STD_LIB_MAX) && (funcIndex <= STD_LIB_FUNC_MAX))
	{
		theLibEntry = (structLibCall*) &( arrayScriptLib[libIndex][funcIndex] );
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

#ifdef CAN_SIGN_TEXT
	else if ((libIndex == LIB_CRYPTO) && (funcIndex == FUNC_CRYPTO_SIGNTEXT))
  {
		si->currTime += FUNC_CRYPTO_SIGNTEXT__timeCost;

		if ((OpS_GetTopIndex( si->RE->OpS ) - FUNC_CRYPTO_SIGNTEXT__nbrOfArgs) < (si->RE->currF->returnSP))
		{
			return ERR_WAE_WMLS_STACK_UNDERFLOW;
		}

  	return Call_Crypto_signText( si, libIndex, funcIndex );
  }
#endif

	else if (libIndex >= WTAI_LIB_PUB && libIndex <= WTAI_PDC_NF )
	{
		UINT8   nbrOfArgs = 0;
		UINT16  timeCost = 1; 
		
		if ( (libIndex == WTAI_LIB_PUB) || 
				 ((libIndex > WTAI_LIB_PUB) && (si->theUserAgent->iUserAgentMode == WTA_USER_AGENT)))
		{
			/* Check that the WMLS file calling a (non-public) WTAI funktion has been loaded by a WTA user agent  */

			if (CheckWTAIScriptCall( libIndex, funcIndex, &nbrOfArgs, &timeCost ))
			{
				/* That there is a corresponding WTAI function to be called via SDL over to the WTAI interface */

				si->currTime += timeCost;

				/* check there is enough operands on the stack for the nbr of arguments to the function */
				if ((OpS_GetTopIndex( si->RE->OpS ) - nbrOfArgs) < (si->RE->currF->returnSP))
				{
					return ERR_WAE_WMLS_STACK_UNDERFLOW;
				}

				/* ok, now create the external call */

				si->returnInfo = SI_RETURN_EXTERNAL;

				if ( (si != NULL) && (! SI_NewReturnParams( si )) )
					return ERR_WAE_WMLS_LIB;

				/* the SIs returnparams struct are to be filled in */
				si->returnParams->islibCall = TRUE;
				si->returnParams->libNbr = libIndex;
				si->returnParams->libFuncIndex = funcIndex;

				si->returnParams->argOpS = OpS_NewFromOpS( si->RE->OpS, nbrOfArgs );
				if (si->returnParams->argOpS == NULL)
					return ERR_WAE_WMLS_LIB;

				return ERR_WAE_WMLS_NONE;
			}
			/* else invalid will be the result (se below) */
		}
		/* else invalid will be the result (se below) */
	}

#ifdef USE_PROPRIETARY_WMLS_LIBS
  else
  {
		UINT8   nbrOfArgs = 0;
	
		if ( CLNTa_hasWMLSLibFunc( libIndex, funcIndex, &nbrOfArgs ) )
		{
			si->currTime += 1;

			/* check there is enough operands on the stack for the nbr of arguments to the function */
			if ((OpS_GetTopIndex( si->RE->OpS ) - nbrOfArgs) < (si->RE->currF->returnSP))
			{
				return ERR_WAE_WMLS_STACK_UNDERFLOW;
			}
#ifdef FEATURE_WAP
            if (libIndex==32000 && funcIndex==103)
			{
				CLNTa_log(1,0,"CallLibraryFunction] Musicbell download detected!!");
				isMusicbellDownload= TRUE;
			}
#ifdef FEATURE_GVM
			if (libIndex==32000 && funcIndex==1)
			{
				extern enumErrorCode Call_Netgame_Gamerun
					(pstructSI si, UINT16 libIndex, UINT8 funcIndex );

				CLNTa_log(1,0,"CallLibraryFunction] Call_Netgame_Gamerun");
				return Call_Netgame_Gamerun(si, libIndex, funcIndex);
			}
#endif //NETGAME
#endif //#ifdef FEATURE_WAP
            {
			
			    return CallProprietaryLibraryFunction( si, libIndex, funcIndex, nbrOfArgs );
            }
		}
  }
#endif

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


enumErrorCode CallExternalLibraryFunction( pstructSI si, UINT16 libIndex, UINT8 funcIndex, UINT8 nbrOfArgs )
{
	si->returnInfo = SI_RETURN_EXTERNAL;

	if ( (si != NULL) && (! SI_NewReturnParams( si )) )
		return ERR_WAE_WMLS_LIB;

	/* the SIs returnparams struct are to be filled in */
	si->returnParams->islibCall = TRUE;
	si->returnParams->libNbr = libIndex;
	si->returnParams->libFuncIndex = funcIndex;

	si->returnParams->argOpS = OpS_NewFromOpS( si->RE->OpS, nbrOfArgs );
	if (si->returnParams->argOpS == NULL)
		return ERR_WAE_WMLS_LIB;

	return ERR_WAE_WMLS_NONE;
}

/* Lang library *************************************************************/

enumErrorCode Call_Lang_abs( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
	pstructVar			op1 = RE_Pop( si->RE );

	libIndex=libIndex; /* just to get rid of a compiler warning */
	funcIndex=funcIndex; /* just to get rid of a compiler warning */

	if (Var_ConvertMethod( CONVERT_INT_FLOAT, op1, NULL )) {
		if (op1->type == typeInteger) {
			op1->val.theInt = ABS(op1->val.theInt);
		}

#ifdef HAS_FLOAT
		else {	/* is of type float */
			op1->val.theFloat = ABS(op1->val.theFloat);
		}
#endif

	}
	else {
		Var_AssignInvalid( op1 );
	}
	OpS_Push( si->RE->OpS, &op1 );

	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Call_Lang_min( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
	pstructVar		op2 = RE_Pop( si->RE );
	pstructVar		op1 = RE_Pop( si->RE );
	pstructVar		result = Var_New();
	BOOL					allOk = TRUE;
#ifdef HAS_FLOAT
	FLOAT32				op1val;
	FLOAT32				op2val;
#endif

	libIndex=libIndex; /* just to get rid of a compiler warning */
	funcIndex=funcIndex; /* just to get rid of a compiler warning */

	if (result == NULL) {
		Var_Delete( &op1 );
		Var_Delete( &op2 );
		return ERR_WAE_WMLS_LIB;
	}

	/* the type checks are ONLY to take care of the requirement: max(45.0 , 45)  = 45.0 */
	if (op1->type != typeFloat) {
		allOk = Var_ConvertMethod( CONVERT_INT_FLOAT, op1, NULL );
	}
	if (allOk) {
		if (op2->type != typeFloat) {
			allOk = Var_ConvertMethod( CONVERT_INT_FLOAT, op2, NULL );
		}
	}

	if (allOk) {
		if ((op1->type == typeFloat) || (op2->type == typeFloat)) {
#ifdef HAS_FLOAT
			op1val = (op1->type == typeFloat) ? (op1->val.theFloat):(op1->val.theInt);
			op2val = (op2->type == typeFloat) ? (op2->val.theFloat):(op2->val.theInt);

			if (op1val <= op2val) {
				Var_AssignVar( result, op1 );
			}
			else {
				Var_AssignVar( result, op2 );
			}
#endif
		}
		else { /* both integers */
			Var_AssignInt( result, (op1->val.theInt <= op2->val.theInt) ? (op1->val.theInt):(op2->val.theInt) );
		}
	}

	if (!allOk) {
		Var_AssignInvalid( result );
	}

	Var_Delete( &op1 );
	Var_Delete( &op2 );

	OpS_Push( si->RE->OpS, &result );

	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Call_Lang_max( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
	pstructVar		op2 = RE_Pop( si->RE );
	pstructVar		op1 = RE_Pop( si->RE );
	pstructVar		result = Var_New();
	BOOL					allOk = TRUE;
#ifdef HAS_FLOAT
	FLOAT32				op1val;
	FLOAT32				op2val;
#endif

	libIndex=libIndex; /* just to get rid of a compiler warning */
	funcIndex=funcIndex; /* just to get rid of a compiler warning */

	if (result == NULL) {
		Var_Delete( &op1 );
		Var_Delete( &op2 );
		return ERR_WAE_WMLS_LIB;
	}

	/* the type checks are ONLY to take care of the requirement: max(45.0 , 45)  = 45.0 */
	if (op1->type != typeFloat) {
		allOk = Var_ConvertMethod( CONVERT_INT_FLOAT, op1, NULL );
	}
	if (allOk) {
		if (op2->type != typeFloat) {
			allOk = Var_ConvertMethod( CONVERT_INT_FLOAT, op2, NULL );
		}
	}

	if (allOk) {
		if ((op1->type == typeFloat) || (op2->type == typeFloat)) {
#ifdef HAS_FLOAT
			op1val = (op1->type == typeFloat) ? (op1->val.theFloat):(op1->val.theInt);
			op2val = (op2->type == typeFloat) ? (op2->val.theFloat):(op2->val.theInt);

			if (op1val >= op2val) {
				Var_AssignVar( result, op1 );
			}
			else {
				Var_AssignVar( result, op2 );
			}
#endif
		}
		else { /* both integers */
			Var_AssignInt( result, (op1->val.theInt >= op2->val.theInt) ? (op1->val.theInt):(op2->val.theInt) );
		}
	}

	if (!allOk) {
		Var_AssignInvalid( result );
	}

	Var_Delete( &op1 );
	Var_Delete( &op2 );

	OpS_Push( si->RE->OpS, &result );

	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Call_Lang_parseInt( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
	pstructVar	op1 = RE_Pop( si->RE );
	pstructVar	result = Var_New();
	INT32				theInt;
	UINT8				notUsed;
	BOOL				isOverflow;

	libIndex=libIndex; /* just to get rid of a compiler warning */
	funcIndex=funcIndex; /* just to get rid of a compiler warning */

	if (result == NULL)
  {
		Var_Delete( &op1 );
		return ERR_WAE_WMLS_LIB;
	}
	if ( (VCR_OK == Var_Convert( op1, typeString )) && op1->val.theString &&
  		String2Int(op1->val.theString, &notUsed,
			&theInt, &isOverflow) && (!isOverflow))
  {
		Var_AssignInt( result, theInt );
	}
	else
  {
		Var_AssignInvalid( result );
	}

	Var_Delete( &op1 );

	OpS_Push( si->RE->OpS, &result );
	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Call_Lang_parseFloat( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
	pstructVar	op1 = RE_Pop( si->RE );
	pstructVar	result = Var_New();

#ifdef HAS_FLOAT
	UINT8								notUsed;
	BOOL								isOverflow;
	FLOAT32							theFloat;

	if (result == NULL)
  {
		Var_Delete( &op1 );
		return ERR_WAE_WMLS_LIB;
	}
	if ( (VCR_OK == Var_Convert( op1, typeString )) && op1->val.theString &&
  		String2Float (op1->val.theString, &notUsed, &theFloat, &isOverflow) &&
       (!isOverflow))
  {
		Var_AssignFloat( result, theFloat );
	}
	else
  {
		Var_AssignInvalid( result );
	}

#else
	if (result == NULL)
  {
		Var_Delete( &op1 );
		return ERR_WAE_WMLS_LIB;
	}
	Var_Delete( &op1 );
	Var_AssignInvalid( result );
#endif

	libIndex=libIndex; /* just to get rid of a compiler warning */
	funcIndex=funcIndex; /* just to get rid of a compiler warning */

	Var_Delete( &op1 );

	OpS_Push( si->RE->OpS, &result );

	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Call_Lang_isInt( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
	enumErrorCode		errorCode;
	pstructVar	op1 = NULL;
	enumVarType	varType;

	libIndex=libIndex; /* just to get rid of a compiler warning */
	funcIndex=funcIndex; /* just to get rid of a compiler warning */

	/* have to check if the var is invalid, then invalid will be the result */
	op1 = RE_Pop( si->RE );
	varType = op1->type;
	OpS_Push( si->RE->OpS, &op1 );

	if (varType == typeInvalid) {
		return ERR_WAE_WMLS_NONE;
	}

	/* not invalid so check if parsable */
	errorCode = CallLibraryFunction(si, 0, 3);	/* Lang.parseInt */

	if ( errorCode == ERR_WAE_WMLS_NONE ) {
		op1 = RE_Pop( si->RE );

		if (op1->type == typeInteger) {
			Var_AssignBool( op1, TRUE );
		}
		else {
			Var_AssignBool( op1, FALSE );
		}

		OpS_Push( si->RE->OpS, &op1 );
		return ERR_WAE_WMLS_NONE;
	}
	else {
		return errorCode;
	}
}


enumErrorCode Call_Lang_isFloat( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
	enumErrorCode		errorCode;
	pstructVar	op1 = NULL;
	enumVarType	varType;

	libIndex=libIndex; /* just to get rid of a compiler warning */
	funcIndex=funcIndex; /* just to get rid of a compiler warning */

	/* have to check if the var is invalid, then invalid will be the result */
	op1 = RE_Pop( si->RE );
	varType = op1->type;
	OpS_Push( si->RE->OpS, &op1 );

	if (varType == typeInvalid) {
		return ERR_WAE_WMLS_NONE;
	}

	/* not invalid so check if parsable */
	errorCode = CallLibraryFunction(si, 0, 4);	/* Lang.parseFloat */

	if ( errorCode == ERR_WAE_WMLS_NONE ) {
		op1 = RE_Pop( si->RE );

		if (op1->type == typeFloat) {
			Var_AssignBool( op1, TRUE );
		}
		else {
			Var_AssignBool( op1, FALSE );
		}

		OpS_Push( si->RE->OpS, &op1 );
		return ERR_WAE_WMLS_NONE;
	}
	else {
		return errorCode;
	}
}


enumErrorCode Call_Lang_maxInt( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
	pstructVar	op1 = Var_New();

	libIndex=libIndex; /* just to get rid of a compiler warning */
	funcIndex=funcIndex; /* just to get rid of a compiler warning */

	if (op1 == NULL) {
		return ERR_WAE_WMLS_LIB;
	}

	Var_AssignInt( op1, INT32_MAX );
	OpS_Push( si->RE->OpS, &op1 );
	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Call_Lang_minInt( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
	pstructVar	op1 = Var_New();

	libIndex=libIndex; /* just to get rid of a compiler warning */
	funcIndex=funcIndex; /* just to get rid of a compiler warning */

	if (op1 == NULL) {
		return ERR_WAE_WMLS_LIB;
	}

	Var_AssignInt( op1, INT32_MIN );
	OpS_Push( si->RE->OpS, &op1 );
	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Call_Lang_Float( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
	pstructVar	op1 = Var_New();

	libIndex=libIndex; /* just to get rid of a compiler warning */
	funcIndex=funcIndex; /* just to get rid of a compiler warning */

	if (op1 == NULL) {
		return ERR_WAE_WMLS_LIB;
	}

#ifdef HAS_FLOAT
	Var_AssignBool( op1, TRUE );
#else
	Var_AssignBool( op1, FALSE );
#endif
	OpS_Push( si->RE->OpS, &op1 );
	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Call_Lang_exit( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
    si = si; /* just to get rid of a compiler warning */
	libIndex=libIndex; /* just to get rid of a compiler warning */
	funcIndex=funcIndex; /* just to get rid of a compiler warning */

	/* does not have to pop because the argument is the result */
	return ERR_WAE_WMLS_EXIT;
}


enumErrorCode Call_Lang_abort( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
	libIndex=libIndex; /* just to get rid of a compiler warning */
	funcIndex=funcIndex; /* just to get rid of a compiler warning */

	/* convert the argument to string */
	CallLibraryFunction(si, 2, 14);	/* Lang.toString */
	/* the errorDescription argument is now the return value */
	return ERR_WAE_WMLS_ABORT;
}


/* Help functions to Call_Lang_random */
#ifndef NO_GLOBAL_VARS
static UINT32 lib_rand_table[55];
static BYTE lib_rand_index1;
static BYTE lib_rand_index2;
static BYTE lib_rand_initialized = 0;
#endif

void lib_rand_seed (UINT32 j)
{
	UINT32 k = 1;
	BYTE i;
	BYTE ii;
	BYTE loop;

	lib_rand_table[54] = j;

	for (i = 0; i < 55; i++)
	{
		ii = 21 * i % 55;
		lib_rand_table[ii] = k;
		k = j - k;
		j = lib_rand_table[ii];
	}
	for (loop = 0; loop < 4; loop++)
	{
		for (i = 0; i < 55; i++)
		{
			lib_rand_table[i] = lib_rand_table[i] - lib_rand_table[(1 + i + 30) % 55];
		}
	}
	lib_rand_index1 = 0;
	lib_rand_index2 = 31;

	lib_rand_initialized++;
}


UINT32 lib_rand(UINT32 limit)
{
	if (!lib_rand_initialized)
	{
		lib_rand_seed(161803398);
		lib_rand_initialized++;
	}
	lib_rand_index1 = (lib_rand_index1 + 1) % 55;
	lib_rand_index2 = (lib_rand_index2 + 1) % 55;
	lib_rand_table[lib_rand_index1] = lib_rand_table[lib_rand_index1] - lib_rand_table[lib_rand_index2];

	return lib_rand_table[lib_rand_index1] % limit;
}


/* - - - - - - - -  - - - - - - - - - */


enumErrorCode Call_Lang_random( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
	pstructVar	op1 = RE_Pop( si->RE );
	INT32				intValue;

	libIndex=libIndex; /* just to get rid of a compiler warning */
	funcIndex=funcIndex; /* just to get rid of a compiler warning */

	if (Var_ConvertMethod( CONVERT_INT_FLOAT, op1, NULL )) {
		if (op1->type == typeFloat) {
			OpS_Push( si->RE->OpS, &op1 );
			CallLibraryFunction( si, 1, 0 ); /* Float.int */
			op1 = RE_Pop( si->RE );
		}

		if (op1->type == typeInteger)
		{
			intValue = op1->val.theInt;

			if (intValue > 0)
      {
				Var_AssignInt( op1, lib_rand(intValue + 1) );
			}
			else if (intValue < 0)
			{
				Var_AssignInvalid( op1 );
			}
			/* else == 0 and then 0 will be the return value */
		}
		else
		{
			Var_AssignInvalid( op1 );
		}
	}
	else {
		Var_AssignInvalid( op1 );
	}

	OpS_Push( si->RE->OpS, &op1 );
	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Call_Lang_seed( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
	pstructVar	op1 = RE_Pop( si->RE );
	INT32				intValue;

	libIndex=libIndex; /* just to get rid of a compiler warning */
	funcIndex=funcIndex; /* just to get rid of a compiler warning */

	if (Var_ConvertMethod( CONVERT_INT_FLOAT, op1, NULL ))
  {
		if (op1->type == typeFloat)
    {
			OpS_Push( si->RE->OpS, &op1 );
			CallLibraryFunction( si, 1, 0 ); /* Float.int */
			op1 = RE_Pop( si->RE );
		}

		if (op1->type == typeInteger)
		{
			intValue = op1->val.theInt;

			if (intValue >= 0) {
				lib_rand_seed( intValue );
			}
			else
			{	/* 000808 (KHN) WAP 1.2.1 update */
				lib_rand_seed( CLNTa_currentTime() + lib_rand_initialized );
			}

			Var_Delete( &op1 );
			op1 = Var_New();	/* an empty string (success) */
		}
		else
		{
			Var_AssignInvalid( op1 );
		}

	}
	else {
		Var_AssignInvalid( op1 );
	}

	OpS_Push( si->RE->OpS, &op1 );
	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Call_Lang_characterSet( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
/*
Function created 990322 by KHN
*/
{
	pstructVar	result = Var_New();

	libIndex=libIndex; /* just to get rid of a compiler warning */
	funcIndex=funcIndex; /* just to get rid of a compiler warning */

	if (result == NULL) {
		return ERR_WAE_WMLS_LIB;
	}

	/* The WMLS interpreter handles all strings internally in
  the unicode (UCS2) format */
	Var_AssignInt( result, (INT32)(IANA_CHARSET_UCS2) );

	OpS_Push( si->RE->OpS, &result );
	return ERR_WAE_WMLS_NONE;
}

/* Float library *************************************************************/


enumErrorCode Call_Float_int( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
	pstructVar	op1 = RE_Pop( si->RE );

	libIndex=libIndex; /* just to get rid of a compiler warning */
	funcIndex=funcIndex; /* just to get rid of a compiler warning */

#ifdef HAS_FLOAT
	if (Var_ConvertMethod( CONVERT_INT_FLOAT, op1, NULL )) {
		if (op1->type == typeFloat) {
			if ((op1->val.theFloat >= FLOAT32_INTMAX_AS_FLOAT) || (op1->val.theFloat < INT32_MIN) ) {
				Var_AssignInvalid( op1 ); /*the float is bigger/smaller than an int can be*/
			}
			else {
				Var_AssignInt( op1, (INT32) op1->val.theFloat );
			}
		}
		/* else int => op1 doesn't need converting */
	}
	else {
		Var_AssignInvalid( op1 );
	}
#else
	Var_AssignInvalid( op1 );
#endif

	OpS_Push( si->RE->OpS, &op1 );
	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Call_Float_floor( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
	pstructVar	op1 = RE_Pop( si->RE );

	libIndex=libIndex; /* just to get rid of a compiler warning */
	funcIndex=funcIndex; /* just to get rid of a compiler warning */

#ifdef HAS_FLOAT
	if (Var_ConvertMethod( CONVERT_INT_FLOAT, op1, NULL )) {
		if (op1->type == typeFloat) {
			if ((op1->val.theFloat >= FLOAT32_INTMAX_AS_FLOAT) || (op1->val.theFloat < INT32_MIN) ) {
				Var_AssignInvalid( op1 ); /*the float is bigger/smaller than an int can be*/
			}
			else {
				Var_AssignInt( op1, (INT32)floor( op1->val.theFloat ) );
			}
		}
		/* else int => op1 doesn't need converting */
	}
	else {
		Var_AssignInvalid( op1 );
	}
#else
	Var_AssignInvalid( op1 );
#endif

	OpS_Push( si->RE->OpS, &op1 );
	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Call_Float_ceil( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
	pstructVar	op1 = RE_Pop( si->RE );

	libIndex=libIndex; /* just to get rid of a compiler warning */
	funcIndex=funcIndex; /* just to get rid of a compiler warning */

#ifdef HAS_FLOAT
	if (Var_ConvertMethod( CONVERT_INT_FLOAT, op1, NULL )) {
		if (op1->type == typeFloat) {
			if ((op1->val.theFloat >= FLOAT32_INTMAX_AS_FLOAT) || (op1->val.theFloat < INT32_MIN) ) {
				Var_AssignInvalid( op1 ); /*the float is bigger/smaller than an int can be*/
			}
			else {
				Var_AssignInt( op1, (INT32) ceil( op1->val.theFloat ) );
			}
		}
		/* else int => op1 doesn't need converting */
	}
	else {
		Var_AssignInvalid( op1 );
	}
#else
	Var_AssignInvalid( op1 );
#endif

	OpS_Push( si->RE->OpS, &op1 );
	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Call_Float_pow( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
	pstructVar	op2 = RE_Pop( si->RE );
	pstructVar	op1 = RE_Pop( si->RE );
	pstructVar	result = Var_New();

	BOOL				op1Z,
							op1LTZ,
							op2LTZ,
							op2Int;
#ifdef HAS_FLOAT
	FLOAT32			mathresult = (FLOAT32)0;
#endif

	libIndex=libIndex; /* just to get rid of a compiler warning */
	funcIndex=funcIndex; /* just to get rid of a compiler warning */

	if (result == NULL) {
		Var_Delete( &op1 );
		Var_Delete( &op2 );
		return ERR_WAE_WMLS_LIB;
	}

#ifdef HAS_FLOAT
	/* clear errno so we can be sure of error/no error when checking later */
  errno = 0;

	if (Var_ConvertMethod( CONVERT_INTS_FLOATS, op1, op2 )) {
		if (op2->type == typeInteger) { /* then both are int */
			op1Z = (op1->val.theInt == 0);
			op1LTZ = (op1->val.theInt < 0);
			op2LTZ = (op2->val.theInt < 0);
			op2Int = TRUE;
		}
		else { /* ops are float */
			op1Z = (op1->val.theFloat == 0);
			op1LTZ = (op1->val.theFloat < 0);
			op2LTZ = (op2->val.theFloat < 0);
			op2Int = (op2->val.theFloat == (INT32)(op2->val.theFloat));
		}

		if ((op1Z && op2LTZ) || (op1LTZ && (!op2Int))) {
			Var_AssignInvalid( result );
		}
		else {
			if (op2->type == typeInteger) { /* then both are int */
				if (op1Z)
        {	/* 0 to the power of x = 1 */
					mathresult = (FLOAT32)1.0;
        }
        else
        {
					mathresult = (FLOAT32) pow((FLOAT32)op1->val.theInt, (FLOAT32)op2->val.theInt);
        }
			}
			else {
				if (op1Z)
        {	/* 0 to the power of x = 1 */
					mathresult = (FLOAT32)1.0;
        }
        else
        {
					mathresult = (FLOAT32) pow(op1->val.theFloat, op2->val.theFloat);
        }
			}
		}
	}
	else {
		Var_AssignInvalid( result );
	}

  if ( ((mathresult != 0) && (errno == ERANGE)) || (errno == EDOM) )
  {	/* math error! */
		Var_AssignInvalid( result );
  }
  else if ( result->type != typeInvalid  )
  {
		Var_AssignFloat( result, mathresult );
  }

 	/* restore errno */
  errno = 0;

#else
	Var_AssignInvalid( result );
#endif

	Var_Delete( &op1 );
	Var_Delete( &op2 );

	OpS_Push( si->RE->OpS, &result );
	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Call_Float_round( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
	pstructVar	op1 = RE_Pop( si->RE );

	libIndex=libIndex; /* just to get rid of a compiler warning */
	funcIndex=funcIndex; /* just to get rid of a compiler warning */

#ifdef HAS_FLOAT
	if (Var_ConvertMethod( CONVERT_INT_FLOAT, op1, NULL )) {
		if (op1->type == typeFloat) {
			if ((op1->val.theFloat >= FLOAT32_INTMAX_AS_FLOAT) || (op1->val.theFloat < INT32_MIN) ) {
				Var_AssignInvalid( op1 ); /*the float is bigger/smaller than an int can be*/
			}
			else
      {
				if (op1->val.theFloat >= 0)
        {
					Var_AssignInt( op1, (INT32)(op1->val.theFloat + 0.5) );
					/* the typecast does trunc and not round, therefore the added 0.5 */
				}
 				else if (( op1->val.theFloat - (INT32)op1->val.theFloat ) < -0.5)
        {	/* -x.5 .. -x.99..., x.5 excluded */
					Var_AssignInt( op1, (INT32)(op1->val.theFloat) -1 );
				}
        else
        {	/* -x.0 .. -x.5, x.5 included */
					Var_AssignInt( op1, (INT32)(op1->val.theFloat) );
        }
			}
		}
		/* else int => op1 doesn't need converting */
	}
	else {
		Var_AssignInvalid( op1 );
	}
#else
	Var_AssignInvalid( op1 );
#endif

	OpS_Push( si->RE->OpS, &op1 );
	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Call_Float_sqrt( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
	pstructVar	op1 = RE_Pop( si->RE );

#ifdef HAS_FLOAT
	FLOAT32			mathresult;
#endif

	libIndex=libIndex; /* just to get rid of a compiler warning */
	funcIndex=funcIndex; /* just to get rid of a compiler warning */

#ifdef HAS_FLOAT
	/* clear errno so we can be sure of error/no error when checking later */
  errno = 0;

	if (Var_ConvertMethod( CONVERT_FLOATS, op1, NULL ))
  {
		if (op1->val.theFloat < 0 )
    {
			Var_AssignInvalid( op1 ); /* Can't be a negative number */
		}
		else
    {
    	mathresult = (FLOAT32) sqrt( op1->val.theFloat );
		}
	}
	else {
		Var_AssignInvalid( op1 );
	}

  if ( ((mathresult != 0) && (errno == ERANGE)) || (errno == EDOM) )
  {	/* math error! */
		Var_AssignInvalid( op1 );
  }
  else if ( op1->type != typeInvalid  )
  {
		Var_AssignFloat( op1, mathresult );
  }
  
 	/* restore errno */
  errno = 0;

#else
	Var_AssignInvalid( op1 );
#endif

	OpS_Push( si->RE->OpS, &op1 );
	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Call_Float_maxFloat( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
	pstructVar	result = Var_New();

	libIndex=libIndex; /* just to get rid of a compiler warning */
	funcIndex=funcIndex; /* just to get rid of a compiler warning */

	if (result == NULL) {
		return ERR_WAE_WMLS_LIB;
	}

#ifdef HAS_FLOAT
	Var_AssignFloat( result, FLOAT32_MAX );
#else
	Var_AssignInvalid( result );
#endif

	OpS_Push( si->RE->OpS, &result );
	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Call_Float_minFloat( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
	pstructVar	result = Var_New();

	libIndex=libIndex; /* just to get rid of a compiler warning */
	funcIndex=funcIndex; /* just to get rid of a compiler warning */

	if (result == NULL) {
		return ERR_WAE_WMLS_LIB;
	}

#ifdef HAS_FLOAT
	Var_AssignFloat( result, FLOAT32_MIN );
#else
	Var_AssignInvalid( result );
#endif

	OpS_Push( si->RE->OpS, &result );
	return ERR_WAE_WMLS_NONE;
}


/* String library *************************************************************/


enumErrorCode Call_String_length( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
	pstructVar		op1 = RE_Pop( si->RE );

	libIndex=libIndex; /* just to get rid of a compiler warning */
	funcIndex=funcIndex; /* just to get rid of a compiler warning */

	if (Var_ConvertMethod( CONVERT_STRINGS, op1, NULL )) {
		Var_AssignInt( op1, (INT32)(op1->theStringLen) );
	}
	else {
		Var_AssignInvalid( op1 );
	}

	OpS_Push( si->RE->OpS, &op1 );
	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Call_String_isEmpty( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
	pstructVar		op1 = RE_Pop( si->RE );

	libIndex=libIndex; /* just to get rid of a compiler warning */
	funcIndex=funcIndex; /* just to get rid of a compiler warning */

	if (Var_ConvertMethod( CONVERT_STRINGS, op1, NULL )) {
		Var_AssignBool( op1, (BOOL)(op1->theStringLen == 0) ); /* empty string */
	}
	else {
		Var_AssignInvalid( op1 );
	}

	OpS_Push( si->RE->OpS, &op1 );
	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Call_String_charAt( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
	pstructVar		op2 = RE_Pop( si->RE );	/* string index */
	pstructVar		op1 = RE_Pop( si->RE );	/* the string */
	pstructVar		result = Var_New();			/* the result is initialized to an empty string */

	libIndex=libIndex; /* just to get rid of a compiler warning */
	funcIndex=funcIndex; /* just to get rid of a compiler warning */

	if (Var_ConvertMethod( CONVERT_INT_FLOAT, op2, NULL )) {
		if (op2->type == typeFloat) {
			OpS_Push( si->RE->OpS, &op2 );
			CallLibraryFunction( si, 1, 0 ); /* Float.int */
			op2 = RE_Pop( si->RE );
		}
		if ( (op2->type == typeInteger) &&
					Var_ConvertMethod( CONVERT_STRINGS, op1, NULL ))
    {
		/* now the op2 (string index) is of the type int */
			if (op2->val.theInt < (INT32)op1->theStringLen)
      {
				/* the index is within the string */
				Var_AssignString( result, 1, &(op1->val.theString[op2->val.theInt]) );
			}
			/* else do nothing since an empty string is the correct result (see var decl.) */
		}
		else Var_AssignInvalid( result );
	}
	else Var_AssignInvalid( result );

	Var_Delete( &op2 );
	Var_Delete( &op1 );

	OpS_Push( si->RE->OpS, &result );
	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Call_String_subString( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
	pstructVar		op3 = RE_Pop( si->RE );	/* substring length */
	pstructVar		op2 = RE_Pop( si->RE );	/* start index */
	pstructVar		op1 = RE_Pop( si->RE );	/* src string */
	pstructVar		result = Var_New();			/* the result is initialized to an empty string */
	INT32					startIndex;
	INT32					stopIndex;
	INT32					subStrLen;

	libIndex=libIndex; /* just to get rid of a compiler warning */
	funcIndex=funcIndex; /* just to get rid of a compiler warning */

	if (Var_ConvertMethod( CONVERT_INTS_FLOATS, op2, op3 )) {
		if (op2->type == typeFloat) { /* then both are float */
			OpS_Push( si->RE->OpS, &op2 );
			CallLibraryFunction( si, 1, 0 ); /* Float.int */
			op2 = RE_Pop( si->RE );
			OpS_Push( si->RE->OpS, &op3 );
			CallLibraryFunction( si, 1, 0 ); /* Float.int */
			op3 = RE_Pop( si->RE );
		}

		if ( (op2->type == typeInteger) && (op3->type == typeInteger) &&
					Var_ConvertMethod( CONVERT_STRINGS, op1, NULL ))
    {
			startIndex = MAX( 0, op2->val.theInt  );
			stopIndex = MIN( MAX(op3->val.theInt, op3->val.theInt + startIndex), (INT32)op1->theStringLen );
				/* 000215,KHN: The "MAX(op3->val.theInt, op3->val.theInt + startIndex)" above
        								is an simplified overflow check. */

			if (startIndex < stopIndex)
      {
				subStrLen = stopIndex - startIndex;
				Var_AssignString( result, (UINT32)subStrLen, &(op1->val.theString[startIndex]) );
			}
			/* else the result is an empty string (as it is already) */
		}
		else Var_AssignInvalid( result );
	}
	else Var_AssignInvalid( result );

	Var_Delete( &op3 );
	Var_Delete( &op2 );
	Var_Delete( &op1 );

	OpS_Push( si->RE->OpS, &result );
	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Call_String_find( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
	pstructVar			op2 = RE_Pop( si->RE );	/* substring */
	pstructVar			op1 = RE_Pop( si->RE );	/* src string */
	pstructVar			result = Var_New();
	pstructStrFind	subPos = NULL;

	libIndex=libIndex; /* just to get rid of a compiler warning */
	funcIndex=funcIndex; /* just to get rid of a compiler warning */

	if ( (VCR_OK == Var_Convert( op1, typeString )) && (VCR_OK == Var_Convert( op2, typeString ))) 
	{
		if (op2->theStringLen == 0) 
		{
			Var_AssignInvalid( result );
		}
		else if (op1->theStringLen == 0)
		{
			Var_AssignInt( result, -1 );
		}
		else 
		{
			subPos = FindSubstrings( op1, op2, FALSE );
			if (subPos != NULL) 
			{
				Var_AssignInt( result, subPos->subStrPos );
			}
			else 
			{
				Var_AssignInvalid( result );
			}
		}
	}
	else Var_AssignInvalid( result );

	StrFind_Delete( &subPos );

	Var_Delete( &op2 );
	Var_Delete( &op1 );

	OpS_Push( si->RE->OpS, &result );
	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Call_String_replace( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
	pstructVar			newSub = RE_Pop( si->RE );	/* newsubstring */
	pstructVar			oldSub = RE_Pop( si->RE );	/* old substring */
	pstructVar			src = RE_Pop( si->RE );	/* src string */
	pstructVar			result = Var_New();
	pstructStrFind	subPos = NULL;
	pstructStrFind	el = NULL;
	INT32						nbrOfReplaces = 0;
	UINT32					newStrSize;
	UINT32					srcIndex = 0;
	UINT32					dstIndex = 0;

	libIndex=libIndex; /* just to get rid of a compiler warning */
	funcIndex=funcIndex; /* just to get rid of a compiler warning */

	if ( (VCR_OK == Var_Convert( src, typeString )) && (VCR_OK == Var_Convert( oldSub, typeString )) && (VCR_OK == Var_Convert( newSub, typeString ))) 
	{
			subPos = FindSubstrings( src, oldSub, TRUE );
			if ((subPos != NULL) && (oldSub->theStringLen != 0)) 
			{
				if (subPos->subStrPos == -1) 
				{
					/* no matches! */
					StrFind_Delete( &subPos );
				}
				else 
				{
					/* now find out how big the new string must be */
					/* ...find out how many substring replaces are to be made */
					el = subPos;
					while (el != NULL) 
					{
						nbrOfReplaces++;
						el = el->next;
					}
				}

				newStrSize = src->theStringLen + ((newSub->theStringLen - oldSub->theStringLen)*nbrOfReplaces);

				/* create and fill out the new string */
				Var_NewString( result, newStrSize );
				el = subPos;
				while (srcIndex < src->theStringLen) 
				{
					if (el != NULL) 
					{
						/* copy the src string until a substring occurs */
						if ((INT32)srcIndex < el->subStrPos) 
						{
							COPYSTRINGN( &(result->val.theString[dstIndex]), &(src->val.theString[srcIndex]),
													el->subStrPos - srcIndex );
							dstIndex += el->subStrPos - srcIndex;
							srcIndex = el->subStrPos;
						}
						/* put the replacement sub string in the new string */
						COPYSTRINGN( &(result->val.theString[dstIndex]), newSub->val.theString,
													newSub->theStringLen );
						srcIndex += oldSub->theStringLen;
						dstIndex += newSub->theStringLen;

						el = el->next;
					}
					else 
					{ /* no more replacements => copy the rest of the src string to the new string */
						COPYSTRINGN( &(result->val.theString[dstIndex]), &(src->val.theString[srcIndex]),
												src->theStringLen - srcIndex );
						dstIndex += src->theStringLen - srcIndex;
						srcIndex = src->theStringLen;
					}
				}
			}
			else 
			{
				Var_AssignInvalid( result );
			}
	}
	else 
	{
		Var_AssignInvalid( result );
	}

	StrFind_Delete( &subPos );

	Var_Delete( &src );
	Var_Delete( &oldSub );
	Var_Delete( &newSub );

	OpS_Push( si->RE->OpS, &result );
	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Call_String_elements( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
/*
990308: (KHN) Implemented the Corrigendum (WMLS lib 12)
*/
{
	pstructVar			separatorStr =	RE_Pop( si->RE );
	pstructVar			str =						RE_Pop( si->RE );
	pstructVar			result =				Var_New();
	pstructVar			separatorChar = Var_New();
	pstructStrFind	listOfMatches = NULL;
	pstructStrFind	el =						NULL;
	INT32						nbrOfElements = 0;

	libIndex=libIndex; /* just to get rid of a compiler warning */
	funcIndex=funcIndex; /* just to get rid of a compiler warning */

	if ((VCR_OK == Var_Convert( str, typeString )) && (VCR_OK == Var_Convert( separatorStr, typeString ))) {
		if (separatorStr->theStringLen > 0) {
			Var_NewString( separatorChar, 1 );
			separatorChar->val.theString[0] = separatorStr->val.theString[0];

			listOfMatches = FindSubstrings( str, separatorChar, TRUE );
			if (listOfMatches->subStrPos != -1) {
				/* now count how many matches were found */
				el = listOfMatches;
				while (el != NULL) {
					nbrOfElements++;
					el = el->next;
				}
				/* when wanting elements the result is the amount of separator matches + 1 */
				nbrOfElements += 1;
			}
			else {
				/* no separator => one element (even if it is an empty string) */
				nbrOfElements = 1;
			}

			Var_AssignInt( result, nbrOfElements );
		}
		else {
			Var_AssignInvalid( result );
		}
	}
	else {
		Var_AssignInvalid( result );
	}

	StrFind_Delete( &listOfMatches );

	Var_Delete( &separatorStr );
	Var_Delete( &str );
	Var_Delete( &separatorChar );

	OpS_Push( si->RE->OpS, &result );
	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Call_String_elementAt( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
	pstructVar			separatorStr =	RE_Pop( si->RE );
	pstructVar			elIndex =				RE_Pop( si->RE );
	pstructVar			str =						RE_Pop( si->RE );
	pstructVar			result =				Var_New();
	pstructVar			separatorChar = Var_New();
	pstructStrFind	listOfMatches = NULL;
	pstructStrFind	el =						NULL;
	INT32						nbrOfElements = 0;
	UINT32					theIndex;
	UINT32					i;
	UINT32					startStrPos = 0;
	UINT32					endStrPos = 0;
	UINT32					newStrLen;
	pstructStrFind	startEl =				NULL; /* the el that has the pos for the start of the element */
	pstructStrFind	endEl =					NULL;	/* the el that has the pos for the end of the element */

	libIndex=libIndex; /* just to get rid of a compiler warning */
	funcIndex=funcIndex; /* just to get rid of a compiler warning */

	if ( (VCR_OK == Var_Convert( str, typeString )) && (VCR_OK == Var_Convert( separatorStr, typeString )) &&
				Var_ConvertMethod(CONVERT_INT_FLOAT, elIndex, NULL))
  {

		if (elIndex->type == typeFloat)
    {
			OpS_Push( si->RE->OpS, &elIndex );
			CallLibraryFunction( si, 1, 0 ); /* Float.int */
			elIndex = RE_Pop( si->RE );
		}

		if ((elIndex->type == typeInteger) && (separatorStr->theStringLen > 0))
    {
			Var_NewString( separatorChar, 1 );
			separatorChar->val.theString[0] = separatorStr->val.theString[0];

			listOfMatches = FindSubstrings( str, separatorChar, TRUE );
			if (listOfMatches->subStrPos != -1)
      {
				/* now count how many matches were found */
				el = listOfMatches;
				while (el != NULL)
        {
					nbrOfElements++;
					el = el->next;
				}
				/* when wanting elements the result is the amount of separator matches + 1 */
				nbrOfElements += 1;
			}
			else
      {
				nbrOfElements = (str->theStringLen == 0) ? (0) : (1);
			}
			/* ensure that the index is within the boundaries */
			theIndex = MAX( 0, elIndex->val.theInt );
			theIndex = MIN( theIndex, (UINT32)(nbrOfElements-1) );

			if (nbrOfElements > 0)
      {
				if (nbrOfElements != 1)
        {
					endEl = listOfMatches;
					i = 0;
					while (i < theIndex)
          {
						/* step forward in the list of matches so the element's boundaries are
								found by getting the separator positions around the element */
						startEl = endEl;
						endEl = endEl->next;
						i++;
					}
					startStrPos = (startEl==NULL) ? ( 0 ):( (startEl->subStrPos)+1 );
					endStrPos   = (endEl==NULL)   ? ( str->theStringLen ):( endEl->subStrPos );

					if (startStrPos <= endStrPos)
          {
						/* create the result string */
						newStrLen = endStrPos - startStrPos;
						Var_NewString( result, newStrLen );
						COPYSTRINGN( result->val.theString, &(str->val.theString[startStrPos]), newStrLen );
					}
					/* else an empty string */
				}
				else
        {
					/* the entire string is the answer */
					Var_Delete( &result );
					result = str;
					str = NULL;
				}
			}
			/* else the string doesn't contain any elements and the result is an empty string */
		}
		else
    {
			Var_AssignInvalid( result );
		}
	}
	else
  {
		Var_AssignInvalid( result );
	}

	StrFind_Delete( &listOfMatches );

	Var_Delete( &separatorStr );
	Var_Delete( &elIndex );
	Var_Delete( &str );
	Var_Delete( &separatorChar );

	OpS_Push( si->RE->OpS, &result );
	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Call_String_removeAt( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
	pstructVar			separatorStr =	RE_Pop( si->RE );
	pstructVar			elIndex =				RE_Pop( si->RE );
	pstructVar			str =						RE_Pop( si->RE );
	pstructVar			result =				Var_New();
	pstructVar			separatorChar = Var_New();
	pstructStrFind	listOfMatches = NULL;
	pstructStrFind	el =						NULL;
	INT32						nbrOfElements = 0;
	UINT32					theIndex;
	UINT32					i;
	UINT32					startStrPos = 0;
	UINT32					endStrPos = 0;
	UINT32					newStrLen;
	pstructStrFind	startEl =				NULL; /* the el that has the pos for the start of the element */
	pstructStrFind	endEl =					NULL;	/* the el that has the pos for the end of the element */

	libIndex=libIndex; /* just to get rid of a compiler warning */
	funcIndex=funcIndex; /* just to get rid of a compiler warning */

	if ( (VCR_OK == Var_Convert( str, typeString )) && (VCR_OK == Var_Convert( separatorStr, typeString )) &&
				Var_ConvertMethod(CONVERT_INT_FLOAT, elIndex, NULL)) {

		if (elIndex->type == typeFloat) {
			OpS_Push( si->RE->OpS, &elIndex );
			CallLibraryFunction( si, 1, 0 ); /* Float.int */
			elIndex = RE_Pop( si->RE );
		}

		if ((elIndex->type == typeInteger) && (separatorStr->theStringLen > 0))
    {
			Var_NewString( separatorChar, 1 );
			separatorChar->val.theString[0] = separatorStr->val.theString[0];

			listOfMatches = FindSubstrings( str, separatorChar, TRUE );
			if (listOfMatches->subStrPos != -1) {
				/* now count how many matches were found */
				el = listOfMatches;
				while (el != NULL) {
					nbrOfElements++;
					el = el->next;
				}
				/* when wanting elements the result is the amount of separator matches + 1 */
				nbrOfElements += 1;
			}
			else {
				nbrOfElements = (str->theStringLen == 0) ? (0) : (1);
			}

			/* ensure that the index is within the boundaries */
			theIndex = MAX( 0, elIndex->val.theInt );
			theIndex = MIN( theIndex, (UINT32)(nbrOfElements-1) );

			if (nbrOfElements > 0) {
				if (nbrOfElements != 1) {
					endEl = listOfMatches;
					i = 0;
					while (i < theIndex) {
						/* step forward in the list of matches so the element's boundaries are
								found by getting the separator positions around the element */
						startEl = endEl;
						endEl = endEl->next;
						i++;
					}
					startStrPos = (startEl==NULL) ? ( 0 ):( (startEl->subStrPos)+1 );
					endStrPos   = (endEl==NULL)   ? ( str->theStringLen-1 ):( endEl->subStrPos );
					if (endStrPos == str->theStringLen-1) {
						/* if the last element is removed the separator is also to be removed */
						startStrPos--;
					}

					if (startStrPos < endStrPos) {
						/* create the result string */
						newStrLen = str->theStringLen - (endStrPos - startStrPos + 1);
						Var_NewString( result, newStrLen );
						/* copy the string part before the element */
						if (startStrPos > 0) {
							COPYSTRINGN( result->val.theString, str->val.theString, startStrPos );
						}
						/* copy the string part after the element */
						if ((endStrPos+1) < str->theStringLen) {
							COPYSTRINGN( &(result->val.theString[startStrPos]),
								&(str->val.theString[endStrPos+1]), (str->theStringLen - (endStrPos+1)) );
						}
					}
					else {
						/* an empty string is to be removed and thus the entire string is the result */
						Var_Delete( &result );
						result = str;
						str = NULL;
					}
				}
				/* else => the entire string is the element to remove so an empty string is the result */
			}
			/* else the string doesn't contain any elements and the result is an empty string */
		}
		else {
			Var_AssignInvalid( result );
		}
	}
	else {
		Var_AssignInvalid( result );
	}

	StrFind_Delete( &listOfMatches );

	Var_Delete( &separatorStr );
	Var_Delete( &elIndex );
	Var_Delete( &str );
	Var_Delete( &separatorChar );

	OpS_Push( si->RE->OpS, &result );
	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Call_String_replaceAt( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
	pstructVar			separatorStr =	RE_Pop( si->RE );
	pstructVar			elIndex =				RE_Pop( si->RE );
	pstructVar			newEl =					RE_Pop( si->RE );
	pstructVar			str =						RE_Pop( si->RE );
	pstructVar			result =				Var_New();
	pstructVar			separatorChar = Var_New();
	pstructStrFind	listOfMatches = NULL;
	pstructStrFind	el =						NULL;
	INT32						nbrOfElements = 0;
	UINT32					theIndex = 0;
	UINT32					i = 0;
	UINT32					startStrPos = 0;
	UINT32					endStrPos = 0;
	UINT32					newStrLen = 0;
	pstructStrFind	startEl =				NULL; /* the el that has the pos for the start of the element */
	pstructStrFind	endEl =					NULL;	/* the el that has the pos for the end of the element */

	libIndex=libIndex; /* just to get rid of a compiler warning */
	funcIndex=funcIndex; /* just to get rid of a compiler warning */

	if ( (VCR_OK == Var_Convert( str, typeString )) && (VCR_OK == Var_Convert( separatorStr, typeString )) &&
			(VCR_OK == Var_Convert( newEl, typeString )) &&	Var_ConvertMethod(CONVERT_INT_FLOAT, elIndex, NULL)) {

		if (elIndex->type == typeFloat) {
			OpS_Push( si->RE->OpS, &elIndex );
			CallLibraryFunction( si, 1, 0 ); /* Float.int */
			elIndex = RE_Pop( si->RE );
		}

		if ((elIndex->type == typeInteger) && (separatorStr->theStringLen > 0))
    {
			Var_NewString( separatorChar, 1 );
			separatorChar->val.theString[0] = separatorStr->val.theString[0];

			listOfMatches = FindSubstrings( str, separatorChar, TRUE );
			if (listOfMatches->subStrPos != -1) {
				/* now count how many matches were found */
				el = listOfMatches;
				while (el != NULL) {
					nbrOfElements++;
					el = el->next;
				}
				/* when wanting elements the result is the amount of separator matches + 1 */
				nbrOfElements += 1;
			}
			else {
				nbrOfElements = (str->theStringLen == 0) ? (0) : (1);
			}

			/* ensure that the index is within the boundaries */
			theIndex = MAX( 0, elIndex->val.theInt );
			theIndex = MIN( theIndex, (UINT32)(nbrOfElements-1) );

			if (nbrOfElements > 1) {
				endEl = listOfMatches;
				i = 0;
				while (i < theIndex) {
					/* step forward in the list of matches so the element's boundaries are
							found by getting the separator positions around the element */
					startEl = endEl;
					endEl = endEl->next;
					i++;
				}
				startStrPos = (startEl==NULL) ? ( 0 ):( (startEl->subStrPos)+1 );
				endStrPos   = (endEl==NULL)   ? ( str->theStringLen-1 ):( (endEl->subStrPos)-1 );
					/* the separator is not part of the replacement */

				/* create the result string */
				newStrLen = str->theStringLen + (newEl->theStringLen - MAX( 0, (INT32)(endStrPos+1 - startStrPos)));/* the endStrPos can be lower than the start (e.g if is the last element and it is empty)  */
				Var_NewString( result, newStrLen );
				/* copy the string part before the element */
				if (startStrPos > 0) {
					COPYSTRINGN( result->val.theString, str->val.theString, startStrPos );
				}
				/* put in the new element instead of the old one */
				if (newEl->theStringLen > 0) {
					COPYSTRINGN( &(result->val.theString[startStrPos]),
						newEl->val.theString, newEl->theStringLen );
				}
				/* copy the string part after the element */
				if ((endStrPos+1) < str->theStringLen) {
					COPYSTRINGN( &(result->val.theString[startStrPos + newEl->theStringLen]),
						&(str->val.theString[endStrPos+1]), (str->theStringLen - (endStrPos+1)) );
				}
			}
			else {
				/* the string will be replaced by the new element */
				Var_Delete( &result );
				result = newEl;
				newEl = NULL;
			}
		}
		else {
			Var_AssignInvalid( result );
		}
	}
	else {
		Var_AssignInvalid( result );
	}

	StrFind_Delete( &listOfMatches );

	Var_Delete( &separatorStr );
	Var_Delete( &elIndex );
	Var_Delete( &newEl );
	Var_Delete( &str );
	Var_Delete( &separatorChar );

	OpS_Push( si->RE->OpS, &result );
	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Call_String_insertAt( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
	pstructVar			separatorStr =	RE_Pop( si->RE );
	pstructVar			elIndex =				RE_Pop( si->RE );
	pstructVar			newEl =					RE_Pop( si->RE );
	pstructVar			str =						RE_Pop( si->RE );
	pstructVar			result =				Var_New();
	pstructVar			separatorChar = Var_New();
	pstructStrFind	listOfMatches = NULL;
	pstructStrFind	el =						NULL;
	INT32						nbrOfElements = 0;
	UINT32					theIndex;
	UINT32					i;
	UINT32					startStrPos = 0;
	UINT32					newStrLen;
	pstructStrFind	startEl =				NULL; /* the el that has the pos for the start of the element */

	libIndex=libIndex; /* just to get rid of a compiler warning */
	funcIndex=funcIndex; /* just to get rid of a compiler warning */

	if ( (VCR_OK == Var_Convert( str, typeString )) && (VCR_OK == Var_Convert( separatorStr, typeString )) &&
			(VCR_OK == Var_Convert( newEl, typeString )) &&	Var_ConvertMethod(CONVERT_INT_FLOAT, elIndex, NULL)) {

		if (elIndex->type == typeFloat) {
			OpS_Push( si->RE->OpS, &elIndex );
			CallLibraryFunction( si, 1, 0 ); /* Float.int */
			elIndex = RE_Pop( si->RE );
		}

		if ((elIndex->type == typeInteger) && (separatorStr->theStringLen > 0))
    {
			Var_NewString( separatorChar, 1 );
			separatorChar->val.theString[0] = separatorStr->val.theString[0];

			listOfMatches = FindSubstrings( str, separatorChar, TRUE );
			if (listOfMatches->subStrPos != -1) {
				/* now count how many matches were found */
				el = listOfMatches;
				while (el != NULL) {
					nbrOfElements++;
					el = el->next;
				}
				/* when wanting elements the result is the amount of separator matches + 1 */
				nbrOfElements += 1;
			}
			else {
				nbrOfElements = (str->theStringLen == 0) ? (0) : (1);
			}

			/* ensure that the index is within the boundaries */
			theIndex = MAX( 0, elIndex->val.theInt );
			theIndex = MIN( theIndex, (UINT32)(nbrOfElements) );
				/* to "append" a new element the index must be able to be "last index + 1" */

			if (nbrOfElements > 0) {

				if (theIndex == 0) {
					startStrPos = 0;
				}
				else if ((INT32)theIndex == nbrOfElements) {
					/* appending an element */
					startStrPos = str->theStringLen;
				}
				else {
					startEl = listOfMatches;
					i = 1;
					while (i < theIndex) {
						/* step forward in the list of matches so the element's boundaries are
								found by getting the separator position */
						startEl = startEl->next;
						i++;
					}
					startStrPos = (startEl==NULL) ? ( 0 ):( (startEl->subStrPos)+1 );
				}

				/* create the result string */
				newStrLen = str->theStringLen + newEl->theStringLen + 1; /* a new separator is needed */
				Var_NewString( result, newStrLen );
				/* copy the string part before the element */
				if (startStrPos > 0) {
					COPYSTRINGN( result->val.theString, str->val.theString, startStrPos );
				}
				/* insert the new element */
				if (startStrPos == str->theStringLen) {
					/* if the new element is inserted at the end, a separatot must be inserted before the element */
					result->val.theString[startStrPos] = separatorChar->val.theString[0];
					startStrPos++; /* the newEl str is now dislocated one step due to this inserted separator
														this dislocation is later removed with "--" (see below) */
				}
				if (newEl->theStringLen > 0) {
					COPYSTRINGN( &(result->val.theString[startStrPos]),
						newEl->val.theString, newEl->theStringLen );
				}
				if (startStrPos != str->theStringLen) {
					/* if not inserted at the end, a separator is inserted after the new element */
					result->val.theString[startStrPos + newEl->theStringLen] = separatorChar->val.theString[0];
					startStrPos++; /* no real meaning but so that the "--" is correct */
				}
				startStrPos--; /* take away the separator dislocation compensation (this is the "--" ;) ) */
				/* copy the string part after the element */
				if (startStrPos < str->theStringLen) {
					COPYSTRINGN( &(result->val.theString[startStrPos + newEl->theStringLen + 1]),
						&(str->val.theString[startStrPos]), (str->theStringLen - startStrPos) );
				}
			}
			else {
				/* the string doesn't contain any elements and the result is the new element */
				Var_Delete( &result );
				result = newEl;
				newEl = NULL;
			}
		}
		else {
			Var_AssignInvalid( result );
		}
	}
	else {
		Var_AssignInvalid( result );
	}

	StrFind_Delete( &listOfMatches );

	Var_Delete( &separatorStr );
	Var_Delete( &elIndex );
	Var_Delete( &newEl );
	Var_Delete( &str );
	Var_Delete( &separatorChar );

	OpS_Push( si->RE->OpS, &result );
	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Call_String_squeeze( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
	pstructVar			str =										RE_Pop( si->RE );
	pstructVar			result =								Var_New(); /* init to an empty string */
	UINT32					newStrLen;
	BOOL						lastCharWasWhitespace	=	FALSE;
	UINT32					srcPos;
	UINT32					dstPos;

	libIndex=libIndex; /* just to get rid of a compiler warning */
	funcIndex=funcIndex; /* just to get rid of a compiler warning */

	if (VCR_OK == Var_Convert( str, typeString )) {
		if (str->theStringLen > 0) {
			/* first find out how long the result will be */
			newStrLen = 0;
			srcPos = 0;
			while (srcPos < str->theStringLen) {
				if ( IsWhitespaceChar(str->val.theString[srcPos]) ) {
					if ( ! lastCharWasWhitespace) {
						newStrLen++;
						lastCharWasWhitespace = TRUE;
					}
				}
				else {
					newStrLen++;
					lastCharWasWhitespace = FALSE;
				}

				srcPos++;
			}
			/* now create the new string */
			if (newStrLen < str->theStringLen) {
				/* whitespaces to be removed */
				Var_NewString( result, newStrLen );
				lastCharWasWhitespace = FALSE;
				dstPos = 0;
				srcPos = 0;
				while (srcPos < str->theStringLen) {
					if ( IsWhitespaceChar(str->val.theString[srcPos]) ) {
						if ( ! lastCharWasWhitespace) {
							result->val.theString[dstPos] = str->val.theString[srcPos];
							dstPos++;
							lastCharWasWhitespace = TRUE;
						}
					}
					else {
						result->val.theString[dstPos] = str->val.theString[srcPos];
						dstPos++;
						lastCharWasWhitespace = FALSE;
					}

					srcPos++;
				}
			}
			else {
				/* no whitespaces to be removed means that the result is the same as str */
				Var_Delete( &result );
				result = str;
				str = NULL;
			}
		}
		/* else the result is an empty string */
	}
	else {
		Var_AssignInvalid( result );
	}

	Var_Delete( &str );

	OpS_Push( si->RE->OpS, &result );
	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Call_String_trim( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
	pstructVar			str =										RE_Pop( si->RE );
	pstructVar			result =								Var_New(); /* init to an empty string */
	UINT32					newStrLen;
	UINT32					firstRealCharPos;
	UINT32					lastRealCharPos;

	libIndex=libIndex; /* just to get rid of a compiler warning */
	funcIndex=funcIndex; /* just to get rid of a compiler warning */

	if (VCR_OK == Var_Convert( str, typeString )) {
		firstRealCharPos = 0;
		while (	(firstRealCharPos < str->theStringLen) &&
						IsWhitespaceChar(str->val.theString[firstRealCharPos]) )
    {
			firstRealCharPos++;
		}

		if (firstRealCharPos < str->theStringLen)
    {
    	lastRealCharPos = str->theStringLen -1;
			while (	(lastRealCharPos > 0) &&
							IsWhitespaceChar(str->val.theString[lastRealCharPos]) )
      {
				lastRealCharPos--;
			}
			if ((firstRealCharPos == 0) && (lastRealCharPos == str->theStringLen -1))
      {
				/* no whitespace to remove thus the result is str */
				Var_Delete( &result );
				result = str;
				str = NULL;
			}
			else
      {
				newStrLen = lastRealCharPos - firstRealCharPos +1;
				Var_NewString( result, newStrLen );
				COPYSTRINGN( result->val.theString, &(str->val.theString[firstRealCharPos]), newStrLen );
			}
		}
		/* else => the whole string is filled with whitespace and the result is then an empty string */
	}
	else {
		Var_AssignInvalid( result );
	}

	Var_Delete( &str );

	OpS_Push( si->RE->OpS, &result );
	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Call_String_compare( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
	pstructVar			str2 =			RE_Pop( si->RE );
	pstructVar			str1 =			RE_Pop( si->RE );
	pstructVar			result =		Var_New(); /* init to an empty string */
	UINT32					i;
	UINT32					minLen;

	libIndex=libIndex; /* just to get rid of a compiler warning */
	funcIndex=funcIndex; /* just to get rid of a compiler warning */

	if ( (VCR_OK == Var_Convert( str1, typeString )) && (VCR_OK == Var_Convert( str2, typeString ))) {
		Var_AssignInt( result, 10 ); /* anything that is not -1, 0 or 1 */
		minLen = MIN ( str1->theStringLen, str2->theStringLen );
		i = 0;
		while ((result->val.theInt == 10) && (i < minLen) ) {
			if (str1->val.theString[i] == str2->val.theString[i]) {
				/* equal */
				i++;
			}
			else if (str1->val.theString[i] < str2->val.theString[i]) {
				/* str1 < str2 */
				Var_AssignInt( result, -1 );
			}
			else {
				/* str1 > str2 */
				Var_AssignInt( result, 1 );
			}
		}

		if (result->val.theInt == 10) {
			/* they matched, but their lengths may be different */
			if (str1->theStringLen == str2->theStringLen) {
				Var_AssignInt( result, 0 );
			}
			else if (str1->theStringLen < str2->theStringLen) {
				/* str1 < str2 */
				Var_AssignInt( result, -1 );
			}
			else {
				/* str1 > str2 */
				Var_AssignInt( result, 1 );
			}
		}
	}
	else {
		Var_AssignInvalid( result );
	}
	Var_Delete( &str2 );
	Var_Delete( &str1 );

	OpS_Push( si->RE->OpS, &result );
	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Call_String_toString( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
	pstructVar			str =										RE_Pop( si->RE );

	libIndex=libIndex; /* just to get rid of a compiler warning */
	funcIndex=funcIndex; /* just to get rid of a compiler warning */

	if (str->type == typeInvalid) {
		Var_NewString( str, 7 );
		str->val.theString[0] = WC('i');
		str->val.theString[1] = WC('n');
		str->val.theString[2] = WC('v');
		str->val.theString[3] = WC('a');
		str->val.theString[4] = WC('l');
		str->val.theString[5] = WC('i');
		str->val.theString[6] = WC('d');
	}
	else {
		if ( ! (VCR_OK == Var_Convert( str, typeString ))) {
			Var_AssignInvalid( str );
		}
	}

	OpS_Push( si->RE->OpS, &str );
	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Call_String_format( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
	pstructVar			value =			RE_Pop( si->RE );
	pstructVar			format =		RE_Pop( si->RE );
	pstructVar			result =		Var_New();
	BOOL						statusOK = TRUE;
	pstructVar			procentSign = Var_New();
	pstructStrFind	listOfProcentSigns = NULL;
	pstructStrFind	el = NULL;
	pstructStrFind	lastEl = NULL;
	INT32						lastmatchPos = 0;
	WCHAR						*aStr = NULL;
	UINT32					pos = 0;
	UINT32					nbrOfChars;
	UINT32					newStrLen = 0;
	BOOL						foundFirstFormatSpec = FALSE;

	WCHAR*					src = NULL;
	WCHAR*					dst = NULL;

	libIndex=libIndex; /* just to get rid of a compiler warning */
	funcIndex=funcIndex; /* just to get rid of a compiler warning */

	Var_NewString( procentSign, 1 );
	procentSign->val.theString[0] = WC('%');

	if (statusOK = (VCR_OK == Var_Convert( format, typeString )))
	{
		listOfProcentSigns = FindSubstrings( format, procentSign, TRUE );

		if (listOfProcentSigns->subStrPos != -1)
		{	/* matches found */

			/* now find %% matches and set its replacement text to % */
			lastmatchPos = -2;	/* so it isn't consecutive with a % in position 0 */
			el = listOfProcentSigns;
			lastEl = listOfProcentSigns;
			while (el != NULL)
			{
				if (el->subStrPos == (INT32)(lastmatchPos + 1))
				{	/* two consecutive % means a single % and not a format specifier */
					aStr = NEWARRAY( WCHAR, 2 );
					aStr[0] = procentSign->val.theString[0];
					aStr[1] = 0;
					lastEl->data = (void*)aStr;
					lastEl->endPos = el->subStrPos + 1;
					aStr = NULL;

					lastEl->next = el->next;
					el->next = NULL;
					StrFind_Delete( &el );
					el = lastEl->next;
					lastmatchPos = -2;	/* make sure more than two consecutive % will be transdlated into one */
				}
				else
				{
					lastmatchPos = el->subStrPos;
					lastEl = el;
					el = el->next;
				}
			}

			/* now find the first % specifier and parse out % [<unsigned int>] [.<unsigned int>] d|f|s */
			el = listOfProcentSigns;
			foundFirstFormatSpec = FALSE;
			statusOK = TRUE;
			newStrLen = format->theStringLen;

			while ((el != NULL) && statusOK)
			{
				if (el->data == NULL)
				{
					pos = el->subStrPos + 1;
					if ( ! foundFirstFormatSpec)
					{
						el->data = (void*)parseFormatSpecifier( format->val.theString, format->theStringLen, &pos, value, &statusOK );
						/* this format specifier (the first) is included in the new string */
						if (statusOK)
						{
								if (el->data != NULL)
								{
									newStrLen += ( (INT32)STRINGLENGTH((WCHAR*)(el->data)) - ( (INT32)pos - el->subStrPos ));
								}
								else
								{	/* the result was an empty string */
									newStrLen -= (INT32)pos - el->subStrPos;
								}

								foundFirstFormatSpec = TRUE;
						}
					}
					else
					{
						aStr = parseFormatSpecifier( format->val.theString, format->theStringLen, &pos, value, &statusOK );
						/* the formatSpec is not to be included in the new string */
						newStrLen -= ( pos - el->subStrPos );
						DEALLOC( &aStr );
					}
					el->endPos = pos;
				}
				else
				{	/* the el was a %% that turned % */
					newStrLen--;
				}

				el = el->next;
			}

			if (statusOK)
			{	/* no incorrect formatSpecifiers. Now it's time to build the result string */
				el = listOfProcentSigns;
				Var_NewString( result, newStrLen );

				src = format->val.theString;
				dst = result->val.theString;

				while (el != NULL)
				{
					nbrOfChars = (format->val.theString + el->subStrPos) - src;
					COPYSTRINGN( dst, src, nbrOfChars );
					dst += nbrOfChars;
					src += nbrOfChars;

					if (el->data != NULL)
					{
						nbrOfChars = STRINGLENGTH( (WCHAR*)el->data );
						COPYSTRINGN( dst, (WCHAR*)el->data, nbrOfChars );

						dst += nbrOfChars;
					}

					src = format->val.theString + el->endPos;

					el = el->next;
				}
				if (src < (format->val.theString  + format->theStringLen))
				{	/* add the last of the format string if any */
					nbrOfChars = (format->val.theString  + format->theStringLen) - src;
					COPYSTRINGN( dst, src, nbrOfChars );
				}

			}

		}
		else
		{	/* no % which means that format is the result*/
			Var_Delete( &result );
			result = format;
			format = NULL;
		}
	}

	if ( ! statusOK)
	{
		Var_AssignInvalid( result );
	}

	StrFind_Delete( &listOfProcentSigns );
	Var_Delete( &procentSign );
	Var_Delete( &format );
	Var_Delete( &value );
	DEALLOC( &aStr );


	OpS_Push( si->RE->OpS, &result );
	return ERR_WAE_WMLS_NONE;
}


/* URL library *************************************************************/

enumErrorCode Call_URL_isValid( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
	pstructVar			str =				RE_Pop( si->RE );
	pstructVar			result =		Var_New();
	BOOL						bResult;

	libIndex=libIndex; /* just to get rid of a compiler warning */
	funcIndex=funcIndex; /* just to get rid of a compiler warning */

	if (VCR_OK == Var_Convert( str, typeString ))
	{
		bResult = w_IsValid( str->val.theString );
		Var_AssignBool( result, bResult );
	}
	else
	{
		Var_AssignInvalid( result );
	}

	Var_Delete( &str );

	OpS_Push( si->RE->OpS, &result );
	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Call_URL_getScheme( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
	pstructVar			str =				RE_Pop( si->RE );
	pstructVar			result =		Var_New();
	WCHAR						*strResult =	NULL;
	UINT32					strLen = 0;

	libIndex=libIndex; /* just to get rid of a compiler warning */
	funcIndex=funcIndex; /* just to get rid of a compiler warning */

	Var_AssignInvalid( result );

	if (VCR_OK == Var_Convert( str, typeString ))
	{
		if ( w_GetScheme(str->val.theString, &strResult) )
		{
    	if (strResult != NULL)
      {
				strLen = STRINGLENGTH( strResult );
				Var_AssignStringDirect( result, strLen, &strResult );
      }
      else
      {	/* the empty string is the result */
				Var_NewString( result, 0 );
      }
		}
	}

	Var_Delete( &str );

	OpS_Push( si->RE->OpS, &result );
	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Call_URL_getHost( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
	pstructVar			str =				RE_Pop( si->RE );
	pstructVar			result =		Var_New();
	WCHAR						*strResult =	NULL;
	UINT32					strLen = 0;


	libIndex=libIndex; /* just to get rid of a compiler warning */
	funcIndex=funcIndex; /* just to get rid of a compiler warning */

	Var_AssignInvalid( result );

	if (VCR_OK == Var_Convert( str, typeString ))
	{
		if ( w_GetHost(str->val.theString, &strResult) )
		{
    	if (strResult != NULL)
      {
				strLen = STRINGLENGTH( strResult );
				Var_AssignStringDirect( result, strLen, &strResult );
      }
      else
      {	/* the empty string is the result */
				Var_NewString( result, 0 );
      }
		}
	}

	Var_Delete( &str );

	OpS_Push( si->RE->OpS, &result );
	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Call_URL_getPort( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
	pstructVar			str =				RE_Pop( si->RE );
	pstructVar			result =		Var_New();
	WCHAR						*strResult =	NULL;
	UINT32					strLen = 0;

	libIndex=libIndex; /* just to get rid of a compiler warning */
	funcIndex=funcIndex; /* just to get rid of a compiler warning */

	Var_AssignInvalid( result );

	if (VCR_OK == Var_Convert( str, typeString ))
	{
		if ( w_GetPort(str->val.theString, &strResult) )
		{
    	if (strResult != NULL)
      {
				strLen = STRINGLENGTH( strResult );
				Var_AssignStringDirect( result, strLen, &strResult );
      }
      else
      {	/* the empty string is the result */
				Var_NewString( result, 0 );
      }
		}
	}

	Var_Delete( &str );

	OpS_Push( si->RE->OpS, &result );
	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Call_URL_getPath( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
	pstructVar			str =				RE_Pop( si->RE );
	pstructVar			result =		Var_New();
	WCHAR						*strResult =	NULL;
	UINT32					strLen = 0;

	libIndex=libIndex; /* just to get rid of a compiler warning */
	funcIndex=funcIndex; /* just to get rid of a compiler warning */

	Var_AssignInvalid( result );

	if (VCR_OK == Var_Convert( str, typeString ))
	{
		if ( w_GetPath(str->val.theString, &strResult) )
		{
    	if (strResult != NULL)
      {
				strLen = STRINGLENGTH( strResult );
				Var_AssignStringDirect( result, strLen, &strResult );
      }
      else
      {	/* the empty string is the result */
				Var_NewString( result, 0 );
      }
		}
	}

	Var_Delete( &str );

	OpS_Push( si->RE->OpS, &result );
	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Call_URL_getParameters( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
	pstructVar			str =				RE_Pop( si->RE );
	pstructVar			result =		Var_New();
	WCHAR						*strResult =	NULL;
	UINT32					strLen = 0;

	libIndex=libIndex; /* just to get rid of a compiler warning */
	funcIndex=funcIndex; /* just to get rid of a compiler warning */

	Var_AssignInvalid( result );

	if (VCR_OK == Var_Convert( str, typeString ))
	{
		if ( w_GetParameters(str->val.theString, &strResult) )
		{
    	if (strResult != NULL)
      {
				strLen = STRINGLENGTH( strResult );
				Var_AssignStringDirect( result, strLen, &strResult );
      }
      else
      {	/* the empty string is the result */
				Var_NewString( result, 0 );
      }
		}
	}

	Var_Delete( &str );

	OpS_Push( si->RE->OpS, &result );
	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Call_URL_getQuery( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
	pstructVar			str =				RE_Pop( si->RE );
	pstructVar			result =		Var_New();
	WCHAR						*strResult =	NULL;
	UINT32					strLen = 0;

	libIndex=libIndex; /* just to get rid of a compiler warning */
	funcIndex=funcIndex; /* just to get rid of a compiler warning */

	Var_AssignInvalid( result );

	if (VCR_OK == Var_Convert( str, typeString ))
	{
		if ( w_GetQuery(str->val.theString, &strResult) )
		{
    	if (strResult != NULL)
      {
				strLen = STRINGLENGTH( strResult );
				Var_AssignStringDirect( result, strLen, &strResult );
      }
      else
      {	/* the empty string is the result */
				Var_NewString( result, 0 );
      }
		}
	}

	Var_Delete( &str );

	OpS_Push( si->RE->OpS, &result );
	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Call_URL_getFragment( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
	pstructVar			str =				RE_Pop( si->RE );
	pstructVar			result =		Var_New();
	WCHAR						*strResult =	NULL;
	UINT32					strLen = 0;

	libIndex=libIndex; /* just to get rid of a compiler warning */
	funcIndex=funcIndex; /* just to get rid of a compiler warning */

	Var_AssignInvalid( result );

	if (VCR_OK == Var_Convert( str, typeString ))
	{
		if ( w_GetFragment(str->val.theString, &strResult) )
		{
    	if (strResult != NULL)
      {
				strLen = STRINGLENGTH( strResult );
				Var_AssignStringDirect( result, strLen, &strResult );
      }
      else
      {	/* the empty string is the result */
				Var_NewString( result, 0 );
      }
		}
	}

	Var_Delete( &str );

	OpS_Push( si->RE->OpS, &result );
	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Call_URL_getBase( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
	pstructVar			result = Var_New();	/* empty string */

	libIndex=libIndex; /* just to get rid of a compiler warning */
	funcIndex=funcIndex; /* just to get rid of a compiler warning */

	if (si->baseURL != NULL)
	{
		Var_AssignString( result, STRINGLENGTH( si->baseURL ), si->baseURL );
	}
	/* ELSE an empty string is returned */

	OpS_Push( si->RE->OpS, &result );
	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Call_URL_getReferer( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
	pstructVar		result = Var_New();
	WCHAR					*resultStr = NULL;
	BOOL					fDummy;
  BYTE					*pbBaseUrl = wip_wchar2byte(si->baseURL, &fDummy);
  BYTE					*pbRefererUrl = wip_wchar2byte(si->refererURL, &fDummy);
  BYTE					*pbResult = b_MinRelativePath( pbBaseUrl, pbRefererUrl, TRUE ); /* 000802 we DO want the fragment (e.g. card name) */

	libIndex=libIndex; /* just to get rid of a compiler warning */
	funcIndex=funcIndex; /* just to get rid of a compiler warning */

	resultStr = wip_byte2wchar(pbResult);
	if (resultStr != NULL)
	{
		Var_AssignStringDirect( result, STRINGLENGTH(resultStr), &resultStr );
	}
  DEALLOC( &pbBaseUrl );
  DEALLOC( &pbRefererUrl );
  DEALLOC( &pbResult );

	OpS_Push( si->RE->OpS, &result );
	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Call_URL_resolve( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
	pstructVar			embeddedUrl =		RE_Pop( si->RE );
	pstructVar			baseUrl =				RE_Pop( si->RE );
	pstructVar			result =				Var_New();
	WCHAR						*strResult =			NULL;
	UINT32					strLen =				0;


	libIndex=libIndex; /* just to get rid of a compiler warning */
	funcIndex=funcIndex; /* just to get rid of a compiler warning */

  Var_AssignInvalid( result );

	if ( (VCR_OK == Var_Convert( baseUrl, typeString )) && (VCR_OK == Var_Convert( embeddedUrl, typeString )))
	{
		if (w_Resolve(baseUrl->val.theString, embeddedUrl->val.theString, &strResult))
		{
    	if (strResult != NULL)
      {
				strLen = STRINGLENGTH( strResult );
				Var_AssignStringDirect( result, strLen, &strResult );
      }
      else
      {	/* the empty string is the result */
				Var_NewString( result, 0 );
      }
		}
	}

	Var_Delete( &embeddedUrl );
	Var_Delete( &baseUrl );

	OpS_Push( si->RE->OpS, &result );
	return ERR_WAE_WMLS_NONE;
}

#ifdef FEATURE_WAP
enumErrorCode Call_URL_escapeString( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
	BYTE*			b_EscapeString (const BYTE* pbString);
	pstructVar		str = RE_Pop( si->RE );
	pstructVar		result = Var_New();	
	WCHAR			*strResult = NULL;
	UINT32			strLen = 0;
	BYTE*			pTmp = NULL;


	libIndex=libIndex; /* just to get rid of a compiler warning */
	funcIndex=funcIndex; /* just to get rid of a compiler warning */

	if (VCR_OK == Var_Convert( str, typeString ))
	{
		if (str->val.theString != NULL)
		{
			Uni2KSCString(str->val.theString, (unsigned char *)escape_buffer);

			pTmp = b_EscapeString((BYTE *)escape_buffer);

			strcpy(result_buffer, (const char *)pTmp );
			
			DEALLOC(&pTmp);

			KSC2UniString((unsigned char*)result_buffer, w_result_buffer);

			Var_AssignString(result, w_strlen(w_result_buffer), w_result_buffer);

    	}
		/* else the result is an empty string */
	}
	else
	{
		Var_AssignInvalid( result );
	}

	Var_Delete( &str );

	OpS_Push( si->RE->OpS, &result );
	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Call_URL_unescapeString( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
	extern			BYTE* b_UnescapeString (const BYTE* pbString);
	pstructVar		str = RE_Pop( si->RE );
	pstructVar		result = Var_New();
	WCHAR			*strResult = NULL;
	UINT32			strLen = 0;
	BYTE*			pTmp = NULL;

	libIndex=libIndex; /* just to get rid of a compiler warning */
	funcIndex=funcIndex; /* just to get rid of a compiler warning */

	if (VCR_OK == Var_Convert( str, typeString ))
	{
		if (str->val.theString != NULL)
		{
			Uni2KSCString(str->val.theString, (unsigned char *)escape_buffer);

			pTmp = (BYTE *)b_UnescapeString((BYTE *)escape_buffer);

			strcpy(result_buffer, (const char *)pTmp );
			
			DEALLOC(&pTmp);

			KSC2UniString((unsigned char*)result_buffer, w_result_buffer);

			Var_AssignString(result, w_strlen(w_result_buffer), w_result_buffer);
		}
		/* else the result is an empty string */
	}
	else
	{
		Var_AssignInvalid( result );
	}

	Var_Delete( &str );

	OpS_Push( si->RE->OpS, &result );
	return ERR_WAE_WMLS_NONE;
}

#else //FEATURE_WAP 
// this is original source!!
enumErrorCode Call_URL_escapeString( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
	pstructVar			str =				RE_Pop( si->RE );
	pstructVar			result =		Var_New();
	WCHAR						*strResult =	NULL;
	UINT32					strLen = 0;


	libIndex=libIndex; /* just to get rid of a compiler warning */
	funcIndex=funcIndex; /* just to get rid of a compiler warning */

	if (VCR_OK == Var_Convert( str, typeString ))
	{
		if (str->val.theString != NULL)
		{
			if ( (strResult = w_WMLS_EscapeString(str->val.theString)) != NULL )
			{
				strLen = STRINGLENGTH( strResult );
				Var_AssignStringDirect( result, strLen, &strResult );
			}
			else
			{
				Var_AssignInvalid( result );
			}
		}
		/* else the result is an empty string */
	}
	else
	{
		Var_AssignInvalid( result );
	}

	Var_Delete( &str );

	OpS_Push( si->RE->OpS, &result );
	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Call_URL_unescapeString( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
	pstructVar			str =				RE_Pop( si->RE );
	pstructVar			result =		Var_New();
	WCHAR						*strResult =	NULL;
	UINT32					strLen = 0;

	libIndex=libIndex; /* just to get rid of a compiler warning */
	funcIndex=funcIndex; /* just to get rid of a compiler warning */

	if (VCR_OK == Var_Convert( str, typeString ))
	{
		if (str->val.theString != NULL)
		{
			if ( (strResult = w_WMLS_UnescapeString(str->val.theString)) != NULL )
			{
				strLen = STRINGLENGTH( strResult );
				Var_AssignStringDirect( result, strLen, &strResult );
			}
			else
			{
				Var_AssignInvalid( result );
			}
		}
		/* else the result is an empty string */
	}
	else
	{
		Var_AssignInvalid( result );
	}

	Var_Delete( &str );

	OpS_Push( si->RE->OpS, &result );
	return ERR_WAE_WMLS_NONE;
}
#endif //FEATURE_WAP

enumErrorCode Call_URL_loadString( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
	pstructVar	contentType = RE_Pop( si->RE );
	pstructVar	url = RE_Pop( si->RE );
	pstructVar	result = NULL;
	WCHAR*			resolvedUrl = NULL;
  BOOL				okContentType = TRUE;
  WCHAR*			contentTypeWStr = NULL;
	WCHAR				contentClass_text[6] = { 't', 'e', 'x', 't', '/', 0 };
  UINT8				contentClass_text_len = (UINT8) w_strlen( (WCHAR*)contentClass_text );


	if ( (VCR_OK == Var_Convert( url, typeString )) && (VCR_OK == Var_Convert( contentType, typeString )))
			/* url must be a valid URL and contentType mustn't include wildcards ('*') */
	{
		/* first check that the contentype belongs to text content group */
		if ((contentType->theStringLen > contentClass_text_len) &&
    		CompareWideStringNoCase(	contentClass_text,
        													contentType->val.theString,
            											contentClass_text_len ) )
    {
			/* sets a variable so we can check after the text slash  */
    	contentTypeWStr = &( contentType->val.theString[ contentClass_text_len ] );
    }
    else
    {
    	okContentType = FALSE;
    }
		/* then check that the rest (after the "text<slash>") of the content
    type string	doesn't contain any wildcards, comma or white space characters */
    /* the pirpose is to see that only one content type is specified */
  	while (okContentType && (*contentTypeWStr != 0))
    {
    	okContentType = (*contentTypeWStr != '*') && (*contentTypeWStr != ',') &&
      		(! IsWhitespaceChar(*contentTypeWStr) );
			contentTypeWStr++;
    }

		if (okContentType)
    {
	  	if (w_Resolve( si->baseURL, url->val.theString, &resolvedUrl ) &&
	      (resolvedUrl != NULL))
	    {
				Var_AssignStringDirect( url, STRINGLENGTH(resolvedUrl), &resolvedUrl );

				OpS_Push( si->RE->OpS, &url );
				OpS_Push( si->RE->OpS, &contentType );

				return CallExternalLibraryFunction( si, libIndex, funcIndex, arrayScriptLib[libIndex][funcIndex].nbrOfArgs );
	    }
		}
  }

  /* if the return above was not reached then the following code will be run
  which will mean that the result becomes 'invalid' */

	Var_Delete( &url );
	Var_Delete( &contentType );

	result = Var_New();
	Var_AssignInvalid( result );

	OpS_Push( si->RE->OpS, &result );
	return ERR_WAE_WMLS_NONE;
}


VOID Lib_LoadStringDone( pstructSI si, INT16 charSet, BYTE **pValueStr, UINT32 strLen )
	/* NOTE: the parameter strLen is in bytes. NewString takes number of characters */
{
	pstructVar			result = Var_New();
	BYTE						*pwchSrc = *pValueStr;
	WCHAR						*pwchDst = NULL;
	INT32						nbrOfLetters = 0;
  BOOL						allOk = TRUE;

	if (charSet == IANA_CHARSET_INVALID)
  {	/* no charset specified, then defaults to Latin 1 */
  	charSet = IANA_CHARSET_LATIN1;
  }

  if ( (allOk = Iana2Unicode_canConvert( charSet )) )
  {
  	nbrOfLetters = Iana2Unicode_calcLen( pwchSrc, charSet, FALSE, strLen, &strLen );
    if ( nbrOfLetters > 0 )
    {
			Var_NewString( result, nbrOfLetters );
			pwchDst = result->val.theString;
      allOk = Iana2Unicode_convert( pwchSrc, charSet, strLen, pwchDst,
      		sizeof(WCHAR)*nbrOfLetters );
    }
    else if (nbrOfLetters == -1)
    {	/* could not convert */
			allOk = FALSE;
    }
    /* else if nbrOfLetters == 0 the empty string will be the result */
  }
  if (! allOk)
  {
  	Var_AssignInvalid( result );
  }

	DEALLOC( pValueStr );

	si->inWaitState = FALSE;
		/* The si is no longer in wait state. The loadString has returned */

	OpS_Push( si->RE->OpS, &result );
}


/* WML Browser library *************************************************************/

enumErrorCode Call_Browser_getVar( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
/*
990401: Added checking the syntax of the variable name. (KHN)
*/
{
	pstructVar				varName = RE_Pop( si->RE );
	pstructVar				result = Var_New();
	WCHAR							*varValue = NULL;

	libIndex=libIndex; /* just to get rid of a compiler warning */
	funcIndex=funcIndex; /* just to get rid of a compiler warning */

	Var_AssignInvalid( result );
  /* invalid is the result if the function is not successful */

	if ( (VCR_OK == Var_Convert( varName, typeString )) && (si->theUserAgent != NULL) )
	{
		if (varName->val.theString != NULL)
		{
			if (WML_CheckVariableSyntax(varName->val.theString, si->theUserAgent->iUserAgentMode == WTA_USER_AGENT ))
      {
				varValue = WML_GetVariable( si->theUserAgent, varName->val.theString );
				if (varValue != NULL)
				{
					/* successfull */
					Var_AssignStringDirect( result, STRINGLENGTH(varValue), &varValue );
				}
        else
        {
        	/* successfull, empty string */
   				Var_NewString( result, 0 );
        }
      }
		}
	}

	Var_Delete( &varName );

	OpS_Push( si->RE->OpS, &result );
	return ERR_WAE_WMLS_NONE;
}


BOOL Setvar_checkSyntax( WCHAR* theString, UINT32 theStringLen )
/* a help function that checks that the string only contains valid XML
 CData. */
{
	UINT32	i;
  for (i=0; i<theStringLen; i++ )
  {
		/* #x9 | #xA | #xD | [#x20-#xD7FF] | [#xE000-#xFFFD] | [#x10000-#x10FFFF] */
		/* since we only use 2 bytes then the last interval is not necessary to check */
  	if ( ! (
    			(theString[i] == 0x9) ||
    			(theString[i] == 0xA) ||
    			(theString[i] == 0xD) ||
    			((theString[i] >= 0x20) && (theString[i] <= 0xD7FF)) ||
    			((theString[i] >= 0xE000) && (theString[i] <= 0xFFFD))
          ) )
    {
    	return FALSE;
    }
  }

  return TRUE;
}

enumErrorCode Call_Browser_setVar( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
/*
990401: Added checking the syntax of the variable name. (KHN)
*/
{
	pstructVar				varVal	= RE_Pop( si->RE );
	pstructVar				varName = RE_Pop( si->RE );
	pstructVar				result = Var_New();


	libIndex=libIndex; /* just to get rid of a compiler warning */
	funcIndex=funcIndex; /* just to get rid of a compiler warning */

	Var_AssignInvalid( result );
  /* invalid is the result if the function is not successful */

	if ( (VCR_OK == Var_Convert( varName, typeString )) && (VCR_OK == Var_Convert( varVal, typeString )) &&
			(si->theUserAgent != NULL) )
	{
		if (varName->val.theString != NULL)
		{
			if (WML_CheckVariableSyntax(varName->val.theString, si->theUserAgent->iUserAgentMode == WTA_USER_AGENT ) &&
 		     Setvar_checkSyntax( varVal->val.theString, varVal->theStringLen ) )
      {
				/* success */
				Var_AssignBool( result, 
						WML_SetVariable( si->theUserAgent, varName->val.theString, varVal->val.theString, 
						si->theUserAgent->iUserAgentMode == WTA_USER_AGENT )
					);
			}
		}
	}

	Var_Delete( &varVal );
	Var_Delete( &varName );

	OpS_Push( si->RE->OpS, &result );
	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Call_Browser_go( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
/*
990125: Changed so that the url in the go is not resolved against the script base URL.
        Now no resolve action is taken.
        (KHN)
990308: Removed the vars parameter (Corr. WMLS libs 12)
				(KHN)
*/
{
	pstructVar				url = RE_Pop( si->RE );
	pstructVar				result = Var_New();

	libIndex=libIndex; /* just to get rid of a compiler warning */
	funcIndex=funcIndex; /* just to get rid of a compiler warning */

	if ((si->theUserAgent != NULL) &&
  	(VCR_OK == Var_Convert( url, typeString )) )
	{
		/* This overides any earlier made go or prev so:
			clear the prev, go and vars parameters in the SI */
		si->WMLBrowser_prev = FALSE;
		DEALLOC( &(si->WMLBrowser_goURL) );

		si->WMLBrowserActionInvoked = TRUE;
		/* transfer the new go params to the SI */
		si->WMLBrowser_goURL = url->val.theString;

		url->val.theString = NULL;
		url->theStringLen = 0;
	}
	else
	{
		Var_AssignInvalid( result );
	}

	Var_Delete( &url );

	OpS_Push( si->RE->OpS, &result );
	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Call_Browser_prev( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
/*
990308: Removed the vars parameter (Corr. WMLS Libs 12).
        (KHN)
*/
{
	pstructVar				result = Var_New();

	libIndex=libIndex; /* just to get rid of a compiler warning */
	funcIndex=funcIndex; /* just to get rid of a compiler warning */

	if ((si->theUserAgent != NULL))
	{
		/* This overides any earlier made go or prev so:
			clear the prev, go and vars parameters in the SI */
		DEALLOC( &(si->WMLBrowser_goURL) );

		si->WMLBrowserActionInvoked = TRUE;
		si->WMLBrowser_prev = TRUE;
	}
	else
	{
		Var_AssignInvalid( result );
	}

	OpS_Push( si->RE->OpS, &result );
	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Call_Browser_newContext( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
	/* this function will return invalid if there is no UA to do NewContext on */
	pstructVar		result = Var_New();	/* initialized to an empty string */

	libIndex=libIndex; /* just to get rid of a compiler warning */
	funcIndex=funcIndex; /* just to get rid of a compiler warning */

	if (si->theUserAgent != NULL)
	{
	   WML_NewContext ( si->theUserAgent );
	/*	WML_EmptyHistoryList( si->theUserAgent );
			WML_EmptyVarTable( si->theUserAgent ); */
	}
	else
	{	/* no user agent (UA) */
		Var_AssignInvalid( result );
	}

	OpS_Push( si->RE->OpS, &result );
	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Call_Browser_getCurrentCard( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
	pstructVar		result = Var_New();
	WCHAR					*minRelUrl = NULL;
  BYTE					*pbBaseUrl = NULL;
  BYTE					*pbMinRelUrl = NULL;
  BOOL					fDummy;

	libIndex=libIndex; /* just to get rid of a compiler warning */
	funcIndex=funcIndex; /* just to get rid of a compiler warning */

	if (si->theUserAgent != NULL)
	{
		pbBaseUrl = wip_wchar2byte(si->baseURL, &fDummy); /* we don't care about overflow */

 		pbMinRelUrl = b_MinRelativePath( pbBaseUrl, si->theUserAgent->pbURLBase, TRUE ); /* includes fragment (i.e. card name) */
    minRelUrl = wip_byte2wchar( pbMinRelUrl );

		if (minRelUrl != NULL)
		{
			Var_AssignStringDirect( result, STRINGLENGTH(minRelUrl), &minRelUrl );
		}
		else
		{
			Var_AssignInvalid( result );
		}
	}
	else
	{	/* no user agent (UA) */
		Var_AssignInvalid( result );
	}

	DEALLOC( &pbBaseUrl );
	DEALLOC( &pbMinRelUrl );
	DEALLOC( &minRelUrl );

	OpS_Push( si->RE->OpS, &result );
	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Call_Browser_refresh( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
	pstructVar		result = NULL;

	if (si->theUserAgent != NULL)
	{
		return CallExternalLibraryFunction( si, libIndex, funcIndex, arrayScriptLib[libIndex][funcIndex].nbrOfArgs );
	}

	/* no user agent */
	result = Var_New();
	Var_AssignInvalid( result );
	OpS_Push( si->RE->OpS, &result );
	return ERR_WAE_WMLS_NONE;
}


/* Dialogs library *************************************************************/

enumErrorCode Call_Dialogs_prompt( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
	pstructVar	defInput = RE_Pop( si->RE );
	pstructVar	message = RE_Pop( si->RE );
	pstructVar	result = NULL;

	if ( (VCR_OK == Var_Convert( message, typeString )) && (VCR_OK == Var_Convert( defInput, typeString )))
	{
		OpS_Push( si->RE->OpS, &message );
		OpS_Push( si->RE->OpS, &defInput );

		return CallExternalLibraryFunction( si, libIndex, funcIndex, arrayScriptLib[libIndex][funcIndex].nbrOfArgs );
	}

	Var_Delete( &defInput );
	Var_Delete( &message );

	result = Var_New();
	Var_AssignInvalid( result );

	OpS_Push( si->RE->OpS, &result );
	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Call_Dialogs_confirm( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
	pstructVar	cancel = RE_Pop( si->RE );
	pstructVar	ok = RE_Pop( si->RE );
	pstructVar	message = RE_Pop( si->RE );
	pstructVar	result = NULL;

	if ( (VCR_OK == Var_Convert( message, typeString )) && (VCR_OK == Var_Convert( ok, typeString )) && (VCR_OK == Var_Convert( cancel, typeString )))
	{
		OpS_Push( si->RE->OpS, &message );
		OpS_Push( si->RE->OpS, &ok );
		OpS_Push( si->RE->OpS, &cancel );

		return CallExternalLibraryFunction( si, libIndex, funcIndex, arrayScriptLib[libIndex][funcIndex].nbrOfArgs );
	}

	Var_Delete( &cancel );
	Var_Delete( &ok );
	Var_Delete( &message );

	result = Var_New();
	Var_AssignInvalid( result );

	OpS_Push( si->RE->OpS, &result );
	return ERR_WAE_WMLS_NONE;
}


enumErrorCode Call_Dialogs_alert( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
	pstructVar	message = RE_Pop( si->RE );
	pstructVar	result = NULL;

	if (VCR_OK == Var_Convert( message, typeString ))
	{
		OpS_Push( si->RE->OpS, &message );

		return CallExternalLibraryFunction( si, libIndex, funcIndex, arrayScriptLib[libIndex][funcIndex].nbrOfArgs );
	}

	Var_Delete( &message );

	result = Var_New();
	Var_AssignInvalid( result );

	OpS_Push( si->RE->OpS, &result );
	return ERR_WAE_WMLS_NONE;
}


#ifdef CAN_SIGN_TEXT
/* Crypto library *************************************************************/

enumErrorCode Call_Crypto_signText( pstructSI si, UINT16 libIndex, UINT8 funcIndex )
{
	pstructVar	keyId = RE_Pop( si->RE );
	pstructVar	keyIdType = RE_Pop( si->RE );
	pstructVar	options = RE_Pop( si->RE );
	pstructVar	stringToSign = RE_Pop( si->RE );

	libIndex=libIndex; /* just to get rid of a compiler warning */
	funcIndex=funcIndex; /* just to get rid of a compiler warning */


	if (	(VCR_OK == Var_Convert( stringToSign, typeString )) &&
				(VCR_OK == Var_Convert( options, typeInteger )) &&
				(VCR_OK == Var_Convert( keyIdType, typeInteger )) &&
				(VCR_OK == Var_Convert( keyId, typeString ))	)
	{
		if ( (options->val.theInt >= 0) && (options->val.theInt <= 7) &&
				(keyIdType->val.theInt >= 0) && (keyIdType->val.theInt <= 2) &&
				((keyId->theStringLen % 20) == 0) && si->theUserAgent )
		{
			if (si->sc = ST_newSignedContent())
			{
				BYTE		*utf8Str;
				UINT16	utf8StrLen = Unicode2UTF8_calcLen( stringToSign->val.theString, stringToSign->theStringLen );
				BYTE		*dummy;

				utf8Str = NEWARRAY( BYTE, utf8StrLen );
				if ( Unicode2UTF8_convert( stringToSign->val.theString, stringToSign->theStringLen, utf8Str, utf8StrLen, &dummy ) )
				{
					BYTE		digestinfo[ 35 ];
					UINT16	diglen = 35;
					CHAR*		pchKeyId = NEWARRAY( CHAR, keyId->theStringLen );
					BOOL		overflowDetected = FALSE;
					UINT16	i = 0;

					while ( !overflowDetected && (i < keyId->theStringLen))
					{
						if (keyId->val.theString[i] <= 0xFF)
						{
							pchKeyId[i] = (CHAR)( keyId->val.theString[i] );
						}
						else
						{
							overflowDetected = TRUE;
						}

						i++;
					}

					if ( ! overflowDetected )
					{
						if ( 0 == ST_createDigestInfo (utf8Str, utf8StrLen, digestinfo, &diglen, (UINT8)(options->val.theInt), si->sc) )
						{
							MMIa_signText( si->theUserAgent->iViewID, si->invokeID, stringToSign->val.theString, digestinfo, diglen, 
								(UINT8)(keyIdType->val.theInt), pchKeyId, (UINT16)(keyId->theStringLen), (UINT8)( options->val.theInt & ~1 ) );

							Var_Delete( &keyId );
							Var_Delete( &keyIdType );
							Var_Delete( &options );
							Var_Delete( &stringToSign );
							DEALLOC( &utf8Str );
							DEALLOC( &pchKeyId );

							si->returnInfo = SI_RETURN_EXTERNAL_NO_PARAMS;
							return ERR_WAE_WMLS_NONE;
						}
					}
					DEALLOC( &pchKeyId );
				}
				DEALLOC( &utf8Str );
			}
		}
	}

	/* something went wrong */
	{
		pstructVar	result = Var_New();

		Var_Delete( &keyId );
		Var_Delete( &keyIdType );
		Var_Delete( &options );
		Var_Delete( &stringToSign );
		if (si->sc)
		{
			ST_freeSignedContent( si->sc );
			si->sc = NULL;
		}

		Var_AssignInvalid( result );
		OpS_Push( si->RE->OpS, &result );
		return ERR_WAE_WMLS_NONE;
	}
}

VOID Lib_textSigned( pstructSI si, UINT8 algoritm, UINT16 sigLen, BYTE *signature, 
											BYTE *hashedKey, UINT16 hashedKeyLen, 
											BYTE *certificate, UINT16 certificateLen, UINT8 certificateType, 
											UINT16 err )
{
	pstructVar	result = Var_New();

	si->inWaitState = FALSE;
		/* The si is no longer in wait state. textSigned has returned */

	if ((si->sc) && (err == 0))
	{
		si->sc->signature.algorithm = algoritm;
		si->sc->signature.siglen = sigLen;
		si->sc->signature.sig = signature;
		signature = NULL;

		if (hashedKey == NULL) 
		{
			si->sc->signerInfo[0].signerInfoType = SIGNERINFO_IMPLICIT;
		}
		else 
		{
			si->sc->signerInfo[0].signerInfoType = SIGNERINFO_SHA_KEY_HASH;
			si->sc->signerInfo[0].info = hashedKey;
			hashedKey = NULL;
			si->sc->signerInfo[0].infoLen = hashedKeyLen;
		}

		if (certificate == NULL) 
		{
			si->sc->signerInfo[1].signerInfoType = SIGNERINFO_IMPLICIT;
		}
		else 
		{
			si->sc->signerInfo[1].signerInfoType = certificateType;
			si->sc->signerInfo[1].info = certificate;
			certificate = NULL;
			si->sc->signerInfo[1].infoLen = certificateLen;
		}

		{
			BYTE		*resultStr = NULL;
			UINT16	resultStrLen = 0;

			if ( 0 == ST_encodeSignedContent( si->sc, &resultStr, &resultStrLen ) )
			{
				WCHAR* pwchResultStr = wip_byte2wchar( resultStr );

				Var_AssignStringDirect(result, resultStrLen, & pwchResultStr );
				OpS_Push( si->RE->OpS, &result );

				DEALLOC( &resultStr );
				ST_freeSignedContent( si->sc );
				si->sc = NULL;
				
				return;
			}

			DEALLOC( &resultStr ); /* just in case */
		}
	}

	/* something went wrong */

	if (si->sc)
	{
		ST_freeSignedContent( si->sc );
		si->sc = NULL;
	}
	DEALLOC( &signature );
	DEALLOC( &hashedKey );
	DEALLOC( &certificate );

	if (err == 1)
	{
		WCHAR noCertStr[] = { 'e','r','r','o','r',':','n','o','C','e','r','t' };

		Var_AssignString( result, 12, noCertStr );
	}
	else if (err == 2)
	{
		WCHAR userCancelStr[] = { 'e','r','r','o','r',':','u','s','e','r','C','a','n','c','e','l' };

		Var_AssignString( result, 16, userCancelStr );
	}
	else
	{
		Var_AssignInvalid( result );
	}
	OpS_Push( si->RE->OpS, &result );
}

/*CAN_SIGN_TEXT*/
#endif

/*****************************************************************************/
#ifdef USE_PROPRIETARY_WMLS_LIBS

WMLSvar* NewWMLSvarFromPstructVar( pstructVar variable )
/* strings will be moved (not copied) from variable to the new WMLSvar */
{
	if (variable)
	{
		WMLSvar *result = NEWARRAY(WMLSvar,1);

		result->type = variable->type;
		result->stringVal = NULL;

		switch (variable->type)
		{
			case typeInteger:
				result->intVal = variable->val.theInt;
				break;

			case typeFloat:
#ifdef HAS_FLOAT
				result->floatVal = variable->val.theFloat;
#else
				result->type = typeInvalid;
#endif
				break;

			case typeString:
				result->stringVal = (CHAR*) variable->val.theString;
#ifdef FEATURE_WAP
                if (isMusicbellDownload)
				{
					if(strcmp(result->stringVal,"1")==0)
					{
						CLNTa_log(1,0,"Attribute arg] Âø½Åº§·Î ÁöÁ¤");
					}
					else if(strcmp(result->stringVal,"2")==0)
					{
						CLNTa_log(1,0,"Attribute arg] Âø½Åº§ ¾Æ´Ô");
					}
					else
					{
						CLNTa_log(1,0,"Attribute arg] broken");
						attributeInvalid = TRUE;
						//w_strcpy(result->stringVal, "1");
					}
					
					isMusicbellDownload = FALSE;
				}
#endif //FEATURE_WAP
				result->stringIANAcharset = IANA_CHARSET_UCS2;
				result->stringLengthInBytes = variable->theStringLen * 2; /* UCS2 characters are two bytes each */
				variable->val.theString = NULL;
				variable->theStringLen = 0;
				break;

			case typeBool:
				result->boolVal = (BOOL) variable->val.theInt;
				break;
		}

		return result;
	}
	else
	{
		return NULL;
	}
}

pstructVar NewPstructVarFromWMLSvar( WMLSvar* variable )
{
	if (variable)
	{
		pstructVar result = Var_New();
    Var_AssignInvalid( result ); /* so the result is invalid if the transfer did not work */

		switch (variable->type)
		{
			case typeInteger:
				Var_AssignInt( result, variable->intVal );
				break;

			case typeFloat:
#ifdef HAS_FLOAT
				Var_AssignFloat( result, variable->floatVal );
#else
				Var_AssignInvalid( result );
#endif
				break;

			case typeString:
				if ( (variable->stringVal != NULL) && (variable->stringLengthInBytes > 0) )
				{
				  BOOL		allOk = TRUE;
					INT32	nbrOfLetters = 0;

					if ( (allOk = Iana2Unicode_canConvert( variable->stringIANAcharset )) )
					{
#ifdef FEATURE_WAP
  					nbrOfLetters = Iana2Unicode_calcLen((BYTE *)variable->stringVal, variable->stringIANAcharset, 
								FALSE, variable->stringLengthInBytes, &variable->stringLengthInBytes );
#else
                    nbrOfLetters = Iana2Unicode_calcLen( variable->stringVal, variable->stringIANAcharset, 
								FALSE, variable->stringLengthInBytes, &variable->stringLengthInBytes );
#endif //FEATURE_WAP

						if ( nbrOfLetters > 0 )
						{
							Var_NewString( result, (UINT32)nbrOfLetters );
#ifdef FEATURE_WAP
							allOk = Iana2Unicode_convert((BYTE *)variable->stringVal, variable->stringIANAcharset, 
									variable->stringLengthInBytes, result->val.theString, sizeof(WCHAR)*nbrOfLetters );
#else
                            allOk = Iana2Unicode_convert( variable->stringVal, variable->stringIANAcharset, 
									variable->stringLengthInBytes, result->val.theString, sizeof(WCHAR)*nbrOfLetters );
#endif //FEATURE_WAP

						}
						else if (nbrOfLetters == -1)
						{	/* could not convert */
							allOk = FALSE;
						}
						else
						{ /* an empty string */
							Var_NewString( result, 0 );
						}
					}
					if (! allOk)
					{
  					Var_AssignInvalid( result );
					}
				}
				else
				{
					Var_NewString( result, 0 );
				}
				break;

			case typeBool:
				Var_AssignBool( result, variable->boolVal );
				break;

			case typeInvalid:
				Var_AssignInvalid( result );
				break;
		}
    return result;
	}
	else
	{
		return NULL;
	}
}


VOID WMLSvar_Delete( WMLSvar** pVariable, BOOL externallyAllocated )
{
	if (*pVariable)
	{
		if (externallyAllocated)
		{
#ifdef USE_WIP_MALLOC
			if ((*pVariable)->stringVal)
			{
				wip_free( ((*pVariable)->stringVal) );
			}
			wip_free( *pVariable );
			*pVariable = NULL;
#else
			if ((*pVariable)->stringVal)
			{
				free( ((*pVariable)->stringVal) );
			}
			free( *pVariable );
			*pVariable = NULL;
#endif
		}
		else
		{
			DEALLOC( & ((*pVariable)->stringVal) );
			DEALLOC( pVariable );
		}
	}
}




enumErrorCode CallProprietaryLibraryFunction( pstructSI si, UINT16 libIndex, UINT8 funcIndex, UINT8 nbrOfArgs )
{
	pstructVar	var = NULL;
	INT16 i;
	WMLSvar** argList = NEWARRAY( WMLSvar*, nbrOfArgs + 1 );
	WMLSvar *returnVal = NULL;
	BOOL	resultLater = FALSE;

	for ( i=0 ; i < nbrOfArgs ; i++)
	{
		var = RE_Pop( si->RE );
		/* fill argList in reverse order since parameters are popped from a stack and
		thus the first parameter is the last to be popped */
		argList[(nbrOfArgs-1) - i] = NewWMLSvarFromPstructVar( var );
		Var_Delete( &var );
	}
	argList[ nbrOfArgs ] = NULL;

	returnVal = CLNTa_callWMLSLibFunc( si->invokeID, libIndex, funcIndex, argList, & resultLater );

	if (returnVal && (!resultLater))
	{
		var = NewPstructVarFromWMLSvar( returnVal );
	}
	if (returnVal)
	{
		WMLSvar_Delete( & returnVal, TRUE );
	}

	/* deallocation */
	for ( i=0 ; i < nbrOfArgs ; i++)
	{
		WMLSvar_Delete( & argList[i], FALSE );
	}
  DEALLOC( &argList );

	if (var && !resultLater)
	{
		OpS_Push( si->RE->OpS, &var );
		return ERR_WAE_WMLS_NONE;
	}
	else if (!resultLater)
	{
		return ERR_WAE_WMLS_LIB;
	}
	else
	{	/* i.e. resultLater == TRUE */
		si->returnInfo = SI_RETURN_EXTERNAL_NO_PARAMS;
		return ERR_WAE_WMLS_NONE;
	}
}


enumErrorCode Lib_ProprietaryLibraryFunctionResponse( pstructSI si, WMLSvar **pResultVar )
{
	pstructVar	WMLSresultVar = NULL;
	
	si->inWaitState = FALSE;
		/* The si is no longer in wait state. the prop.libfunc has returned */

	if (*pResultVar)
	{
		WMLSresultVar = NewPstructVarFromWMLSvar( *pResultVar );
		WMLSvar_Delete( pResultVar, FALSE );
	}

	if (WMLSresultVar)
	{
		OpS_Push( si->RE->OpS, &WMLSresultVar );
		return ERR_WAE_WMLS_NONE;
	}
	else
	{
		return ERR_WAE_WMLS_LIB;
	}
}


/* USE_PROPRIETARY_WMLS_LIBS */
#endif
