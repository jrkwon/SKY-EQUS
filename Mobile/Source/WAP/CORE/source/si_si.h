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
File: si_si.h + c
Author: Klas Hermodsson (KHN)

Description:
NOTE: This description assumes knowledge of the purpose and tasks
of the WMLScript interpreter described in the WMLS specification.
The comments found here are more of an explanation of the design and 
implementation of this interpreter.

The script interpreter (SI) is the component that handles and interprets 
a single script file. When a script file calls an function residing in 
another script file the SI signals this to the Script Queue (see si_sq files) 
which manages the SIs.

A very important component of the SI is the runtime environment (RE, see si_re files).
The RE keeps track of such things as the binary script file itself (BP), 
instruction pointer (IP), operand stack (OpS) and so on.

SI creation
To create the SI it needs the binary script file and the calling information 
(function name and call parameters). There are two different cases when 
a SI needs to be created: a user agent call a script function or another script file
calls a function in the new script file. 
If it is a user agent initiated call then function name and parameters are in the
fragment of the call URL. (SI_NewWithArgString)
If the call came from another script file then the function 
name is separated and the parameters are passed as an operand stack 
just containg those parameter values. (SI_NewWithArgOpS)

SI and "external resources"
Since the SI only handles one script file, a call to an external script function 
will result in a new SI being created. This is one case of needing 
an external resource. When the SI coes upon the external call it stores the
external call data in the returnParams struct. The data stored is
URL, function name and parameters to the external function.

The other case is when a script library function is not entirely 
coded in C but needs help from SDL to complete the task.
In this case the data stored in the returnParams struct is
the library and function index of the function to be completed.

The data saved in the returnParams struct is in time fetched
by the WMLS SDL process and taken care of.
The following functions are used for fetching this data:
SI_GetLibCalllParams,
SI_GetExtCalllParams_URL,
SI_GetExtCalllParams_rest

From the moment that the external resource is needed until 
the result is returned, the SI is in a wait state.
The result is returned via the functions:
SI_ReceiveReturnVal,
SI_LoadStringDone,
SI_ReceiveCalledSIReturnVal

The GO and PREV actions
The library functions WMLBrowser.go and prev are special in the
way that they do not take effect until the whole script invoke is done.
This means that these parameters must be passed back to the caller.
The GO and PREV functions overwrite each other which means that if
there first is a GO call and then a PREV call, the PREV action
overrides all former GO or PREV actions. This means that
if a script file is done and returns a GO or PREV action to the
script that called it, this action overrides the stored GO or PREV
action (if any). Confusing, eh?

How the SI works
When the SI is created, a runtime envrionment (RE) is created (see si_re files).
The central functions of the SI are the constructors and the SI_ExecBlock function.
The SI_ExecBlock function interprets a number of instructions until it reaches the
timeslice (explained below) maximum or an external resource is needed in which case 
the SI is put in a waut state.
The SI_ExecBlock has a return value which signals if the script is done or if
the ExecBlock function needs to be called further. It also signals if an 
external resource is needed.

Each instruction and script library function has been given a time cost.
This is to indicate how much time units it takes to interpret the instruction/lib function.
Timeslice is then a number of time units for which the SI is allowed to interpret 
the script file before returning control to the caller of SI_ExecBlock.
By doing this, the SI doesn't run until it's done but can instead easily
be run "pseudo simultaneously" with other tasks of the WAP client.
The timeslice parameter is configurable and is part of the porting parameters
found in confvars.h.

************************/

#ifndef _SI_SI_H
#define _SI_SI_H


#include "cmmnrsrc.h"
#include "si_re.h"
#include "si_ops.h"

#include "wmldef.h"
	/* def..h is for the pUA type */

#ifdef CAN_SIGN_TEXT
	#include "si_crpto.h"
#endif


typedef enum { 
	SI_RETURN_DONE,					/* The SI is done */
	SI_RETURN_MORE,					/* The SI is not done */
	SI_RETURN_EXTERNAL,			/* The SI will need external resources (extended info will be found in a 
															structSIReturnParams) */
	SI_RETURN_EXTERNAL_NO_PARAMS	/* The SI is waiting for an external result. 
																		No params need to be transferred. The result will later come to the WMLS SDL
																		process and then a C function (depends on what external operation was performed) 
																		will be called. */
} enumSIReturn;

typedef struct
{
	BOOL				islibCall;	/* TRUE = lib call, FALSE = call to an extrenal function (in another BP) */
	pstructOpS	argOpS;			/* the arguments for the library/external function */
	/*------*/
	UINT16			libNbr;
	UINT8				libFuncIndex;
	/*------*/
	WCHAR*			bpUrl;			/* NULL terminated */
	BYTE*				funcName;		/* NULL terminated */

} structSIReturnParams, *pstructSIReturnParams;


typedef struct
{
		/* theUserAgent = the UA that started the script execution.
			This is needed when wanting to access WML variables. */
	pUA											theUserAgent; 	
	UINT8										invokeID; /* the invokeId of the invoke element. This is to have a trace back to where in the script Q the si is */
	pstructRuntimeEnv				RE;
	WCHAR										*baseURL;			/* the absolute url of this compilation */
	WCHAR										*refererURL;	/* the absolute url of the refering page/(script unit) */
	UINT16									timeSlice;
	UINT16									currTime;
	BOOL										inWaitState;	/* TRUE if the SI is waiting for external resources */
	enumSIReturn						returnInfo;
	pstructSIReturnParams		returnParams;
	BOOL										WMLBrowserActionInvoked;	/* TRUE if prev or go invoked */
	BOOL										WMLBrowser_prev;	/* TRUE if the UA should perform a PREV action upon return */
	WCHAR										*WMLBrowser_goURL;	/* if != NULL then the UA should perform the GO action with this URL  */

#ifdef CAN_SIGN_TEXT
	SignedContent						*sc; /* a struct needed when performing the WMLS library function Crypto.signText (see si_libs.c) */
#endif

} structSI, *pstructSI;




/*==========================================
	SI_NewWithArgString
============================================

---Purpose: 
To create a SI for the supplied script file. The called function and its parameter values is
inside the baseUrl parameter (see call URL syntax in the WMLS language spec).
This function is used when the caller is a user agent.

---Params:
BP					The binary script file.
BPlen				The length of BP in bytes.
IANAcharset	The WSP charset header. This might indicate
							the character set used for string constants.
userAgent		The user agent struct. Used for example to access
							WML variables.
baseUrl			The absolute URL of this script file and with a
							fragment which contains function name and parameter values.
refererUrl	The absolute URL to the referring file. Without the card fragment?
timeSlice		The number of time units maximum that SI_ExecBlock can
							run without interruption.
errCode			Returns an error code (see errcodes.h).
	
---Return:
pstructSI		The created SI.
NULL				Something went wrong.

------------------------------------------------------------------------*/
pstructSI SI_NewWithArgString( UINT8 invokeID, BYTE* BP, UINT32 BPlen, INT16 IANAcharset, pUA userAgent, WCHAR *baseUrl, /* this url includes funcname and arguments in the fragment */
															WCHAR *refererUrl, UINT16 timeSlice, enumErrorCode *errCode );


/*==========================================
	SI_NewWithArgOpS
============================================

---Purpose: 
To create a SI for the supplied script file. The function name and parameter values are 
supplied as two separate arguments to this function.
This function is used when the caller is another SI.

---Params:
BP					The binary script file.
BPlen				The length of BP in bytes.
IANAcharset	The WSP charset header. This might indicate
							the character set used for string constants.
userAgent		The user agent struct. Used for example to access
							WML variables.
pFuncName		The name of the function to be called.
pArgOpS			An operand stack containing the argument values to the function called.
baseUrl			The absolute URL of this script file and with a
							fragment which contains function name and parameter values.
refererUrl	The absolute URL to the referring file. Without the card fragment?
timeSlice		The number of time units maximum that SI_ExecBlock can
							run without interruption.
errCode			Returns an error code (see errcodes.h).
	
---Return:
pstructSI		The created SI.
NULL				Something went wrong.

------------------------------------------------------------------------*/
pstructSI SI_NewWithArgOpS( UINT8 invokeID, BYTE* BP, UINT32 BPlen, INT16 IANAcharset, pUA userAgent, BYTE **pFuncName, pstructOpS *pArgOpS,
														WCHAR *baseUrl, WCHAR* refererUrl, UINT16 timeSlice, enumErrorCode *errCode );


/*==========================================
	SI_Delete
============================================

---Purpose: 
Deallocates the SI and its components.

---Params:
pThis		The SI.
	
---Return:
-

------------------------------------------------------------------------*/
VOID SI_Delete( pstructSI *pThis );

/*==========================================
	SI_ExecBlock
============================================

---Purpose: 
The central function of the SI.
This function interprets instructions until either
timeslice time units have been consumed or an 
external resource is needed in which case the SI
goes in to a wait state until the result is
received.

---Params:
thisx		The SI.
	
---Return:
ERR_WAE_WMLS_NONE			All went ok. SI->returnInfo must be checked in order to see
												if the script is done, need external resource or there
												is more to be interpreted.
otherwise							An error ocurred (see errcodes.h and WMLS language spec).

------------------------------------------------------------------------*/

enumErrorCode SI_ExecBlock( pstructSI thisx );
	/* If the return val is SI_RETURN_DONE then the return info is found inside the si (see structSI) */

/*==========================================
	SI_GetReturnVal
============================================

---Purpose: 
If SI->returnInfo == SI_RETURN_DONE (and no errors ocurred) then the 
script file is done and the caller should retrieve a return value 
by calling this function.

---Params:
thisx				The SI.
	
---Return:
pstructVar	The return value.
NULL				something went wrong. E.g. the script is not done.

------------------------------------------------------------------------*/
pstructVar SI_GetReturnVal( pstructSI thisx );

/*==========================================
	SI_ReceiveReturnVal
============================================

---Purpose:
This function is used for returning the result from a script library
function that was completed in SDL.

---Params:
thisx				The SI.
pRetVal			The return value.
							NOTE: The SI will take care of deallocation of this parameter.
	
---Return:
-

------------------------------------------------------------------------*/
VOID SI_ReceiveReturnVal( pstructSI thisx, pstructVar *pRetVal );

/*==========================================
	SI_LoadStringDone
============================================

---Purpose: 
This function is used for returning result from the
URL.loadString library function. 
SI_ReceiveReturnVal can not be used since there is more to be done
before putting it on the operand stack. For instance, it must
be translated to unicode if not already in that charset.

---Params:
thisx				The SI.
charSet			The IANA mibEnum code for the charset used in the string.
pValueStr		The loaded string.
strLen			The length of ValueStr in bytes.
	
---Return:
-

------------------------------------------------------------------------*/
VOID SI_LoadStringDone( pstructSI thisx, INT16 charSet, BYTE **pValueStr, UINT32 strLen );

/*==========================================
	SI_ReceiveCalledSIReturnVal
============================================

---Purpose: 
This function is used for returning the result of calling an
external (not in the same script file) script function.
The result is a return value, and possible a browser action (GO or PREV).
If there is a browser action then it overrides any previous
browser action stored in this SI.

---Params:
thisx									The SI.
pRetVal								The return value to be pushed on the operand stack.
browserActionInvoked	TRUE: a browser action was invoked (either GO or PREV, not both).
BrowserPrev						TRUE: a PREV action was invoked.
pBrowserGoUrl					Not NULL: The URL parameter of the GO action.
	
---Return:
-

------------------------------------------------------------------------*/
VOID SI_ReceiveCalledSIReturnVal( pstructSI thisx, pstructVar *pRetVal, BOOL browserActionInvoked, 
																 BOOL BrowserPrev, WCHAR **pBrowserGoUrl );


/*==========================================
	SI_GetLibCalllParams
============================================

---Purpose: 
When SI->returnInfo == SI_RETURN_EXTERNAL and the SI->returnParams->islibCall is TRUE
then the SI needs SDL to complete a script library function. This function should 
thus be used by SDL to retrieve the data about the lib call such as library and
function index as well as parameter values to the lib func.

---Params:
thisx					The SI.
pLibIndex			Returns the script library index.
pFuncIndex		Returns the script library function index.
pArgOpS				Returns an operand stack containing
								the call parameter values.
	
---Return:
TRUE:					All went well.
FALSE:				Something went wrong. E.g. no external lib call had been made.

------------------------------------------------------------------------*/
BOOL SI_GetLibCalllParams( pstructSI thisx, UINT16 *pLibIndex, UINT8 *pFuncIndex,pstructOpS *pArgOpS );


/*==========================================
	SI_GetExtCalllParams_URL
============================================

---Purpose: 
To retrieve the URL to the script file where the external function 
resides. The SQ calls this function so it can order the file to
be loaded. Not until the file has been loaded is it time to fetch
the function name and the parameter values of the call.
Therefore are they not fetched in this function. The rest can be
retrieved by calling SI_GetExtCalllParams_URL.

---Params:
thisx			This SI.
pCallUrl	Returns the absolute URL to the file 
						where the external function resides.
	
---Return:
TRUE:			Everything went well.
FALSE:		Something went wrong.

------------------------------------------------------------------------*/
BOOL SI_GetExtCalllParams_URL( pstructSI thisx, WCHAR **pCallUrl );


/*==========================================
	SI_GetExtCalllParams_rest
============================================

---Purpose: 
To retrieve the function name and parameter values in the
call to the external script function. 
See SI_GetExtCalllParams_URL above for more info.

---Params:
thisx				This SI.
pFuncName		Returns the function name.
pArgOpS			Returns an operand stack containing
							the parameter values of the call.
	
---Return:
TRUE:				
FALSE:			

------------------------------------------------------------------------*/
BOOL SI_GetExtCalllParams_rest( pstructSI thisx, BYTE **pFuncName, pstructOpS *pArgOpS );


/*==========================================
	SI_NewReturnParams
============================================

---Purpose: 
Creates a struct in the SI to be filled out when
needing an external resource (external function or lib call).
Creates the struct and initialises the attributes. Does not
create a new operand stack!

Used by si_instr and si_libs.

---Params:
si		The SI.
	
---Return:
TRUE:			Everything went well.
FALSE:		Something went wrong.

------------------------------------------------------------------------*/
BOOL SI_NewReturnParams( pstructSI si );

/* _SI_SI_H */
#endif
/*==========================================
	x
============================================

---Purpose: 

---Params:
	
---Return:

------------------------------------------------------------------------*/
