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
#ifndef _SI_LIBS_H
#define _SI_LIBS_H
/***********************
File: si_libs.h + c
Author: Klas Hermodsson (KHN)

Description:
This module contains handling for WMLScript
library functions. The CallLibraryFunction function
below is the main function. 

Note:
Any WMLScript library function can be executed by calling the CallLibraryFunction 
method below although implementation of some are not located in the si_libs.c file.
Public WTAI library functions are located in si_wtai.
WTAI libraray functions are located in wtai_if.

Design explanation:
This module resembles the si_instr module in the way that there
is a main function (CallLibraryFunction) that from two numbers 
(libIndex & funcIndex) calls the appropriate library function.

The library functions are uniquely identified by two numbers, a library index
and a function index. This module has a two-dimensional array where the entry 
found at x,y is the library function with x as library index and y as function index.
The array entry is a structLibCall struct as declared below.

structLibCall explanation:

func
A function pointer to the corresponding implementation of the standard
library function.

timeCost
The cost for executing this library function. Used by the SI (see si_si)
to control length of script execution before interruption. For more info
see si_si files.

nbrOfArgs
The number of arguments used by the library function. This number is used
by the CallLibraryFunction to see, before library function execution, if
a stack underflow will occur.


************************/

#include "cmmnrsrc.h"
#include "si_si.h"
#include "confvars.h"

#ifdef USE_PROPRIETARY_WMLS_LIBS
	#include "aapiclnt.h"
	#include "tapiclnt.h"
#endif

typedef struct {
	enumErrorCode		(*func)( pstructSI, UINT16, UINT8 );
	UINT16			timeCost;
	UINT8				nbrOfArgs;
} structLibCall;


enumErrorCode CallLibraryFunction( pstructSI si, UINT16 libIndex, UINT8 funcIndex );

VOID Lib_LoadStringDone( pstructSI si, INT16 charSet, BYTE **pValueStr, UINT32 strLen );

#ifdef CAN_SIGN_TEXT
VOID Lib_textSigned( pstructSI si, UINT8 algoritm, UINT16 sigLen, BYTE *signature, 
											BYTE *hashedKey, UINT16 hashedKeyLen, 
											BYTE *certificate, UINT16 certificateLen, UINT8 certificateType, 
											UINT16 err );
#endif


/* Lang library *************************************************************/
enumErrorCode Call_Lang_abs( pstructSI si, UINT16 libIndex, UINT8 funcIndex );
enumErrorCode Call_Lang_min( pstructSI si, UINT16 libIndex, UINT8 funcIndex );
enumErrorCode Call_Lang_max( pstructSI si, UINT16 libIndex, UINT8 funcIndex );
enumErrorCode Call_Lang_parseInt( pstructSI si, UINT16 libIndex, UINT8 funcIndex );
enumErrorCode Call_Lang_parseFloat( pstructSI si, UINT16 libIndex, UINT8 funcIndex );
enumErrorCode Call_Lang_isInt( pstructSI si, UINT16 libIndex, UINT8 funcIndex );
enumErrorCode Call_Lang_isFloat( pstructSI si, UINT16 libIndex, UINT8 funcIndex );
enumErrorCode Call_Lang_maxInt( pstructSI si, UINT16 libIndex, UINT8 funcIndex );
enumErrorCode Call_Lang_minInt( pstructSI si, UINT16 libIndex, UINT8 funcIndex );
enumErrorCode Call_Lang_Float( pstructSI si, UINT16 libIndex, UINT8 funcIndex );
enumErrorCode Call_Lang_exit( pstructSI si, UINT16 libIndex, UINT8 funcIndex );
enumErrorCode Call_Lang_abort( pstructSI si, UINT16 libIndex, UINT8 funcIndex );
enumErrorCode Call_Lang_random( pstructSI si, UINT16 libIndex, UINT8 funcIndex );
enumErrorCode Call_Lang_seed( pstructSI si, UINT16 libIndex, UINT8 funcIndex );
enumErrorCode Call_Lang_characterSet( pstructSI si, UINT16 libIndex, UINT8 funcIndex );
/* Float library *************************************************************/
enumErrorCode Call_Float_int( pstructSI si, UINT16 libIndex, UINT8 funcIndex );
enumErrorCode Call_Float_floor( pstructSI si, UINT16 libIndex, UINT8 funcIndex );
enumErrorCode Call_Float_ceil( pstructSI si, UINT16 libIndex, UINT8 funcIndex );
enumErrorCode Call_Float_pow( pstructSI si, UINT16 libIndex, UINT8 funcIndex );
enumErrorCode Call_Float_round( pstructSI si, UINT16 libIndex, UINT8 funcIndex );
enumErrorCode Call_Float_sqrt( pstructSI si, UINT16 libIndex, UINT8 funcIndex );
enumErrorCode Call_Float_maxFloat( pstructSI si, UINT16 libIndex, UINT8 funcIndex );
enumErrorCode Call_Float_minFloat( pstructSI si, UINT16 libIndex, UINT8 funcIndex );
/* String library *************************************************************/
enumErrorCode Call_String_length( pstructSI si, UINT16 libIndex, UINT8 funcIndex );
enumErrorCode Call_String_isEmpty( pstructSI si, UINT16 libIndex, UINT8 funcIndex );
enumErrorCode Call_String_charAt( pstructSI si, UINT16 libIndex, UINT8 funcIndex );
enumErrorCode Call_String_subString( pstructSI si, UINT16 libIndex, UINT8 funcIndex );
enumErrorCode Call_String_find( pstructSI si, UINT16 libIndex, UINT8 funcIndex );
enumErrorCode Call_String_replace( pstructSI si, UINT16 libIndex, UINT8 funcIndex );
enumErrorCode Call_String_elements( pstructSI si, UINT16 libIndex, UINT8 funcIndex );
enumErrorCode Call_String_elementAt( pstructSI si, UINT16 libIndex, UINT8 funcIndex );
enumErrorCode Call_String_removeAt( pstructSI si, UINT16 libIndex, UINT8 funcIndex );
enumErrorCode Call_String_replaceAt( pstructSI si, UINT16 libIndex, UINT8 funcIndex );
enumErrorCode Call_String_insertAt( pstructSI si, UINT16 libIndex, UINT8 funcIndex );
enumErrorCode Call_String_squeeze( pstructSI si, UINT16 libIndex, UINT8 funcIndex );
enumErrorCode Call_String_trim( pstructSI si, UINT16 libIndex, UINT8 funcIndex );
enumErrorCode Call_String_compare( pstructSI si, UINT16 libIndex, UINT8 funcIndex );
enumErrorCode Call_String_toString( pstructSI si, UINT16 libIndex, UINT8 funcIndex );
enumErrorCode Call_String_format( pstructSI si, UINT16 libIndex, UINT8 funcIndex );
/* URL library *************************************************************/
enumErrorCode Call_URL_isValid( pstructSI si, UINT16 libIndex, UINT8 funcIndex );
enumErrorCode Call_URL_getScheme( pstructSI si, UINT16 libIndex, UINT8 funcIndex );
enumErrorCode Call_URL_getHost( pstructSI si, UINT16 libIndex, UINT8 funcIndex );
enumErrorCode Call_URL_getPort( pstructSI si, UINT16 libIndex, UINT8 funcIndex );
enumErrorCode Call_URL_getPath( pstructSI si, UINT16 libIndex, UINT8 funcIndex );
enumErrorCode Call_URL_getParameters( pstructSI si, UINT16 libIndex, UINT8 funcIndex );
enumErrorCode Call_URL_getQuery( pstructSI si, UINT16 libIndex, UINT8 funcIndex );
enumErrorCode Call_URL_getFragment( pstructSI si, UINT16 libIndex, UINT8 funcIndex );
enumErrorCode Call_URL_getBase( pstructSI si, UINT16 libIndex, UINT8 funcIndex );
enumErrorCode Call_URL_getReferer( pstructSI si, UINT16 libIndex, UINT8 funcIndex );
enumErrorCode Call_URL_resolve( pstructSI si, UINT16 libIndex, UINT8 funcIndex );
enumErrorCode Call_URL_escapeString( pstructSI si, UINT16 libIndex, UINT8 funcIndex );
enumErrorCode Call_URL_unescapeString( pstructSI si, UINT16 libIndex, UINT8 funcIndex );
enumErrorCode Call_URL_loadString( pstructSI si, UINT16 libIndex, UINT8 funcIndex );
/* WML Browser library *************************************************************/
enumErrorCode Call_Browser_getVar( pstructSI si, UINT16 libIndex, UINT8 funcIndex );
enumErrorCode Call_Browser_setVar( pstructSI si, UINT16 libIndex, UINT8 funcIndex );
enumErrorCode Call_Browser_go( pstructSI si, UINT16 libIndex, UINT8 funcIndex );
enumErrorCode Call_Browser_prev( pstructSI si, UINT16 libIndex, UINT8 funcIndex );
enumErrorCode Call_Browser_newContext( pstructSI si, UINT16 libIndex, UINT8 funcIndex );
enumErrorCode Call_Browser_getCurrentCard( pstructSI si, UINT16 libIndex, UINT8 funcIndex );
enumErrorCode Call_Browser_refresh( pstructSI si, UINT16 libIndex, UINT8 funcIndex );
/* Dialogs library *************************************************************/
enumErrorCode Call_Dialogs_prompt( pstructSI si, UINT16 libIndex, UINT8 funcIndex );
enumErrorCode Call_Dialogs_confirm( pstructSI si, UINT16 libIndex, UINT8 funcIndex );
enumErrorCode Call_Dialogs_alert( pstructSI si, UINT16 libIndex, UINT8 funcIndex );

#ifdef CAN_SIGN_TEXT
	/* Crypto library *************************************************************/
	enumErrorCode Call_Crypto_signText( pstructSI si, UINT16 libIndex, UINT8 funcIndex );
#endif

#ifdef USE_PROPRIETARY_WMLS_LIBS
enumErrorCode CallProprietaryLibraryFunction( pstructSI si, UINT16 libIndex, UINT8 funcIndex, UINT8 nbrOfArgs );
enumErrorCode Lib_ProprietaryLibraryFunctionResponse( pstructSI si, WMLSvar **pResultVar );
VOID WMLSvar_Delete( WMLSvar** pVariable, BOOL externallyAllocated );
#endif


#endif
