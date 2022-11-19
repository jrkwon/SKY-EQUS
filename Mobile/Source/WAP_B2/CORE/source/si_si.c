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
000316 (KHN) Added handling for a new type of returnInfo in the SI struct. 
               (i.e. SI_RETURN_EXTERNAL_NO_PARAMS)
               This was made in order to recognize the situation when
							 an adaptor call has been made and the script is now waiting
							 for a result without passing back parameters to SDL.
						 Also added calling SI_New... functions with invokeID as a new
						   parameter. This was done to enable script lib functions to
							 call adaptorfunctions directly without having to return
							 parameters to SDL when it is possible to make the call in C.

**********/
#include "si_si.h"

#include "si_instr.h"
#include "si_types.h"
#include "si_libs.h"
#include "URL.h"
#include "si_misc.h"


enumErrorCode ExecInstruction( pstructSI thisx )
{
	BYTE	instr = 0;

	if (RE_ReadInstruction( thisx->RE, &instr )) {
		return ExecBytecodeInstr( instr, thisx );
	}
	else {
		/* We have reached the end of the function
		without executing a return statement. This causes us to return
		with an empty string (see "WML Script Spec" 30-apr 9.3.2) */
		
		return ExecBytecodeInstr( RETURN_ES, thisx );
	}
}

VOID SI_DeleteReturnParams( pstructSIReturnParams *pThis )
{
	if (*pThis != NULL) {
		DEALLOC( &((*pThis)->bpUrl) );
		DEALLOC( &((*pThis)->funcName) );
		OpS_Delete( &((*pThis)->argOpS) );

		DEALLOC( pThis );
	}
}


enumErrorCode SI_ExecBlock( pstructSI thisx )
{
	enumErrorCode				currStatus = ERR_WAE_WMLS_NONE;
	
	if ((thisx != NULL) && (thisx->RE != NULL)) {

		thisx->returnInfo = SI_RETURN_MORE;	/* there is still more instructions to execute */
		SI_DeleteReturnParams( &(thisx->returnParams) );	/* making sure that no old params are around */
		thisx->currTime = 0;		/* this is a new round of execution (a new timeslice to spend) */

		while (	(thisx->currTime < thisx->timeSlice) &&			/* The timeslice is not used up */
						(currStatus == ERR_WAE_WMLS_NONE) &&							/* No error has occured  */
						(thisx->returnInfo == SI_RETURN_MORE)) {		/* The execution is not done or 
																													needing external resources */
			currStatus = ExecInstruction( thisx );
		}

		/* 
		if we need an external resource after this block then we are in a waiting state 
		until a result is returned to us (see SI_ReceiveReturnVal) 
		*/
		thisx->inWaitState = (currStatus == ERR_WAE_WMLS_NONE) && 
			((thisx->returnInfo == SI_RETURN_EXTERNAL) || (thisx->returnInfo == SI_RETURN_EXTERNAL_NO_PARAMS));

		return currStatus;
	}
	else return ERR_WAE_WMLS_NULL;
}



BOOL SI_GetExtCalllParams_URL( pstructSI thisx, WCHAR **pCallUrl )
{
	if ((thisx != NULL) && (*pCallUrl == NULL) &&
			(thisx->inWaitState) && (thisx->returnParams != NULL) &&
			( ! thisx->returnParams->islibCall) && (thisx->returnParams->bpUrl != NULL) )
	{
		*pCallUrl = thisx->returnParams->bpUrl;
		thisx->returnParams->bpUrl = NULL;
		return TRUE;
	}
	return FALSE;
}



BOOL SI_GetExtCalllParams_rest( pstructSI thisx, BYTE **pFuncName, pstructOpS *pArgOpS )
{
	if ((thisx != NULL) && (*pFuncName == NULL) && (*pArgOpS == NULL) &&
			(thisx->inWaitState) && (thisx->returnParams != NULL) &&
			( ! thisx->returnParams->islibCall) && (thisx->returnParams->bpUrl == NULL) &&
			(thisx->returnParams->funcName != NULL) && (thisx->returnParams->argOpS != NULL) )
	{	/* NOTE: checking that bpUrl == NULL so that the callUrl has been picked up first */
		*pFuncName = thisx->returnParams->funcName;
		thisx->returnParams->funcName = NULL;
		*pArgOpS = thisx->returnParams->argOpS;
		thisx->returnParams->argOpS = NULL;
		return TRUE;
	}
	return FALSE;
}


BOOL SI_GetLibCalllParams( pstructSI thisx, UINT16 *pLibIndex, UINT8 *pFuncIndex, pstructOpS *pArgOpS )
{
	if ((thisx != NULL) && (*pArgOpS == NULL) &&
			(thisx->inWaitState) &&
			(thisx->returnParams != NULL) && 
			(thisx->returnParams->islibCall) &&
			(thisx->returnParams->argOpS != NULL) ) 
	{
		*pLibIndex = thisx->returnParams->libNbr;
		*pFuncIndex = thisx->returnParams->libFuncIndex;
		*pArgOpS = thisx->returnParams->argOpS;
		thisx->returnParams->argOpS = NULL;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


pstructVar SI_GetReturnVal( pstructSI thisx )
{
	if ((thisx != NULL) && (thisx->RE != NULL)) {
		return OpS_Pop( thisx->RE->OpS );
	}

	return NULL;
}


pstructSI SI_New( UINT8 invokeID, pstructRuntimeEnv *pRe, pUA userAgent, UINT16 timeSlice, WCHAR *base, WCHAR *referer )
{
	pstructSI				result;
													/* taken away because so to not create memory leakage
														 if timeslice == 0 and the RE is lost*/
	if ((*pRe != NULL)			/* && (timeSlice > 0) */) {
		result = NEWSTRUCT( structSI );

		result->theUserAgent = userAgent;
		result->invokeID = invokeID;
		result->RE = *pRe;
		result->timeSlice = timeSlice;
		result->currTime = 0;
		result->inWaitState = FALSE;
		result->returnInfo = SI_RETURN_MORE;
		result->returnParams = NULL;
		result->WMLBrowserActionInvoked = FALSE;
		result->WMLBrowser_prev = FALSE;
		result->WMLBrowser_goURL = NULL;


		*pRe = NULL;					/* so that the reference is not used by mistake somewhere else */


		if (base != NULL)
		{
			result->baseURL = NEWARRAY( WCHAR, STRINGLENGTH(base)+1 );
			COPYSTRING( result->baseURL, base );
		}
		else
		{
			result->baseURL = NULL;
		}
		if (referer != NULL)
		{
			result->refererURL = NEWARRAY( WCHAR, STRINGLENGTH(referer)+1 );
			COPYSTRING( result->refererURL, referer );
		}
		else
		{
			result->refererURL = NULL;
		}

    /*
    the RE was successfully created and the script file was verified.
    Now the User Agent Pragmas (if any) must be handed over to the user agent.
    (This handover can not be performed until verification/access checks have been made
    but before actual execution of the script has started)
		*/
		BPI_SendUApragmasToTheUA( result->RE->BPI, result->RE->BP,
    		result->RE->BPend, result->theUserAgent );

		return result;
	}

	return NULL;
}


pstructSI SI_NewWithArgString( UINT8 invokeID, BYTE* BP, UINT32 BPlen, INT16 IANAcharset,
		pUA userAgent, WCHAR *baseUrl, /* this url includes funcname and arguments in the fragment */
		WCHAR *refererUrl, UINT16 timeSlice, enumErrorCode *errCode )
{
	pstructSI						result = NULL;
	pstructRuntimeEnv		re = NULL;

	UINT32	i;
	UINT32	baseStrLen;
	/* the baseUrl string will be modified by changing the '#'
  to a string termination in order to  save memory and performance,
  thus the modification must be restored since the baseUrl is only a "loan" */
	UINT32	fragmentMark = 0;

  WCHAR		*pwchFragment = NULL;
  /* to hold the unescaped fragment (contains function name and arguments) */

	WCHAR		*pwchFuncNameStart = NULL;
	WCHAR		*pwchFuncNameEnd = NULL;
	BYTE		*theFuncName = NULL;
	WCHAR		*theArgList = NULL;

	BOOL		funcNameAndArgListOK = FALSE;


	if (baseUrl != NULL)
	{
		/* first find the function name */
		baseStrLen = STRINGLENGTH( baseUrl );
		i = 0;
		while (i < baseStrLen && ((baseUrl[i] != (WCHAR)('#'))))
		{
			i++;
		}
		if (baseUrl[i] == (WCHAR)('#'))
		{	/* found the start of the fragment which contains function name and arglist */
			/*
			now set the fragment anchor '#' to NULL so that baseUrl string
			is now only the url without the fragment
			*/
			baseUrl[i] = 0;
			/* remember the position of the modification so we can restore it later */
			fragmentMark = i;

			i++;	/* step past the place of the '#' */
			if (i < baseStrLen)
			{
				pwchFragment = w_UnescapeString( &(baseUrl[i]) );
			}
		}

		if (pwchFragment != NULL)
		{
      pwchFuncNameStart = pwchFragment;
    	/* the start of the function name was found */
			/* now find the argList start paranthesis */
			theArgList = pwchFragment;
			while (theArgList && ((*theArgList != (WCHAR)('('))))
			{
				theArgList++;
			}
			if (*theArgList == (WCHAR)('('))
			{	/* found the start of the arglist */
				pwchFuncNameEnd = theArgList;    /* the '(' */
				theArgList++;	/* step past the place of the '(' */
				if (*theArgList != 0)
				{	/* we have not come to the end of the string */	
					/* 
					all went ok. Now extract the function name to an US ascii string 
					without leading and trailing blanks 
					*/
					/* locate real end of funcname */
					while ((pwchFuncNameEnd > pwchFuncNameStart) &&
          			IsWhitespaceChar( *(pwchFuncNameEnd-1)) )
					{
						pwchFuncNameEnd--;
					}
					while ((pwchFuncNameStart <= pwchFuncNameEnd) &&
          		IsWhitespaceChar(*pwchFuncNameStart) )
					{
						pwchFuncNameStart++;
					}
					if (pwchFuncNameEnd > pwchFuncNameStart)
					{	/* funcName does consist of more than blanks */
						/* now check that the function name does not consist of non US-ascii chars (above 0x007F) */
						i = 0;
						while (((pwchFuncNameStart + i) < pwchFuncNameEnd)
            		&& (*(pwchFuncNameStart + i) <= 0x007F))
						{
							i++;
						}
						if ((pwchFuncNameStart + i) == pwchFuncNameEnd)
						{	/* all chars valid */
							/* create the funcName string */
							theFuncName = NEWARRAY( BYTE, (pwchFuncNameEnd - pwchFuncNameStart)+1 );
							i = 0;
							while ((pwchFuncNameStart + i) < pwchFuncNameEnd)
							{
								theFuncName[i] = (BYTE) *(pwchFuncNameStart + i);
								i++;
							}
							theFuncName[i] = 0;	/* NULL terminate theFuncName */

							/* finally done with funcName and argList!!! */
							funcNameAndArgListOK = TRUE;
						}
					}
				}
			}
		}

		if (funcNameAndArgListOK)
		{
			re = RE_NewWithArgString( BP, BPlen, IANAcharset, theFuncName, theArgList, refererUrl, baseUrl, errCode  );

			if (re != NULL) {
				result = SI_New( invokeID, &re, userAgent, timeSlice, baseUrl, refererUrl );
			}
		}
		else
		{	/* incorrect callStr (function name and/or arguments) */
			*errCode = ERR_WAE_WMLS_VERIFICATION;
		}
	}
	
	DEALLOC( &theFuncName );
	DEALLOC( &pwchFragment );

	baseUrl[ fragmentMark ] = (WCHAR)('#');
  /* restore the baseUrl parameter */

	return result;
}

pstructSI SI_NewWithArgOpS( UINT8 invokeID, BYTE* BP, UINT32 BPlen, INT16 IANAcharset, pUA userAgent,
		BYTE **pFuncName, pstructOpS *pArgOpS, WCHAR *baseUrl, WCHAR* refererUrl,
    UINT16 timeSlice, enumErrorCode *errCode )
{
	pstructRuntimeEnv		re = NULL;

	re = RE_NewWithArgOpS( BP, BPlen, IANAcharset, *pFuncName, pArgOpS, refererUrl, baseUrl, errCode );

	DEALLOC( pFuncName );

	if (re != NULL) {
		return SI_New( invokeID, &re, userAgent, timeSlice, baseUrl, refererUrl );
	}
	else
	{
		return NULL;
	}
}


VOID SI_Delete( pstructSI *pThis )
{
	if (*pThis != NULL) {
		RE_Delete( &((*pThis)->RE) );
		SI_DeleteReturnParams( &((*pThis)->returnParams) );
		DEALLOC( &((*pThis)->baseURL) );
		DEALLOC( &((*pThis)->refererURL) );
		DEALLOC( &((*pThis)->WMLBrowser_goURL) );

		DEALLOC( pThis );
	}
}


BOOL SI_NewReturnParams( pstructSI thisx )
{
	if (thisx != NULL) {
		SI_DeleteReturnParams( &(thisx->returnParams) );

		thisx->returnParams = NEWSTRUCT( structSIReturnParams );
		thisx->returnParams->argOpS = NULL;
		thisx->returnParams->bpUrl = NULL;
		thisx->returnParams->funcName = NULL;
	}
	return ( (thisx != NULL) && (thisx->returnParams != NULL) );
}



VOID SI_ReceiveReturnVal( pstructSI thisx, pstructVar *pRetVal )
{
	if ((thisx != NULL) && (thisx->inWaitState) && (*pRetVal != NULL))
	{
		OpS_Push( thisx->RE->OpS, pRetVal );
		thisx->inWaitState = FALSE;
	}
}



VOID SI_LoadStringDone( pstructSI thisx, INT16 charSet, BYTE **pValueStr, UINT32 strLen )
{
	Lib_LoadStringDone( thisx, charSet, pValueStr, strLen );
}




VOID SI_ReceiveCalledSIReturnVal( pstructSI thisx, pstructVar *pRetVal, BOOL browserActionInvoked,
																 BOOL BrowserPrev, WCHAR **pBrowserGoUrl )
{
	if ((thisx != NULL) && (thisx->inWaitState) && (*pRetVal != NULL))
	{
		OpS_Push( thisx->RE->OpS, pRetVal );
		thisx->inWaitState = FALSE;

		if (browserActionInvoked)
		{
			thisx->WMLBrowserActionInvoked = TRUE;
			if (BrowserPrev)
			{
				DEALLOC( &(thisx->WMLBrowser_goURL) );
				DEALLOC( &(thisx->WMLBrowser_goURL) );

				thisx->WMLBrowser_prev = TRUE;
			}
			else
			{
				thisx->WMLBrowser_prev = FALSE;
				DEALLOC( &(thisx->WMLBrowser_goURL) );
				thisx->WMLBrowser_goURL = *pBrowserGoUrl;
				*pBrowserGoUrl = NULL;
			}
		}
	}
}
