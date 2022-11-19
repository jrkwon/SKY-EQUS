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
#ifndef _SI_SI_H
#define _SI_SI_H


#include "cmmnrsrc.h"
#include "si_re.h"
#include "si_ops.h"

#include "WMLDef.h"
	/* def..h is for the pUA type */


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

} structSI, *pstructSI;


pstructSI SI_NewWithArgString( UINT8 invokeID, BYTE* BP, UINT32 BPlen, INT16 IANAcharset, pUA userAgent, WCHAR *baseUrl, /* this url includes funcname and arguments in the fragment */
															WCHAR *refererUrl, UINT16 timeSlice, enumErrorCode *errCode );
pstructSI SI_NewWithArgOpS( UINT8 invokeID, BYTE* BP, UINT32 BPlen, INT16 IANAcharset, pUA userAgent, BYTE **pFuncName, pstructOpS *pArgOpS,
														WCHAR *baseUrl, WCHAR* refererUrl, UINT16 timeSlice, enumErrorCode *errCode );
VOID SI_Delete( pstructSI *pThis );

enumErrorCode SI_ExecBlock( pstructSI thisx );
	/* If the return val is SI_RETURN_DONE then the return info is found inside the si (see structSI) */
pstructVar SI_GetReturnVal( pstructSI thisx );
VOID SI_ReceiveReturnVal( pstructSI thisx, pstructVar *pRetVal );

VOID SI_LoadStringDone( pstructSI thisx, INT16 charSet, BYTE **pValueStr, UINT32 strLen );

VOID SI_ReceiveCalledSIReturnVal( pstructSI thisx, pstructVar *pRetVal, BOOL browserActionInvoked, 
																 BOOL BrowserPrev, WCHAR **pBrowserGoUrl );
BOOL SI_GetLibCalllParams( pstructSI thisx, UINT16 *pLibIndex, UINT8 *pFuncIndex,pstructOpS *pArgOpS );

BOOL SI_GetExtCalllParams_URL( pstructSI thisx, WCHAR **pCallUrl );
BOOL SI_GetExtCalllParams_rest( pstructSI thisx, BYTE **pFuncName, pstructOpS *pArgOpS );

BOOL SI_NewReturnParams( pstructSI si );

/* _SI_SI_H */
#endif
