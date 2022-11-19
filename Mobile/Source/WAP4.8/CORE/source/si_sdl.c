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
#include "si_sdl.h"

#include "cmmnrsrc.h"
#include "si_sq.h"
#include "si_var.h"

#ifdef USE_PROPRIETARY_WMLS_LIBS
	#include "si_libs.h"
#endif



void* sdl2c_Init( void )
{
	UINT16	timeSlice				= cfg_wmls_timeSlice;
	BOOL		roundRobin			= cfg_wmls_roundRobin;
	BOOL		oneScriptPerUA	= cfg_wmls_oneScriptPerUa;
	BOOL		handlesTopPrio	= cfg_wmls_handleTopPriority;
	
	return( (void*)(SQ_Init( timeSlice, roundRobin, oneScriptPerUA, handlesTopPrio)) );
}


void sdl2c_Terminate( void **pThis )
{
	SQ_Terminate( (pstructSQ*) pThis );
}



long sdl2c_InvokeScript( void* thisx, unsigned long UApidAsInt, void* theUA, void** pBP,
		unsigned long BPlen, long IANAcharset, void** pCallURL, long isTopPriority )
{
	return( (long) SQ_InvokeScript( (pstructSQ) thisx, (UINT32) UApidAsInt,
  		(pUA) theUA, (BYTE**) pBP, (UINT32) BPlen, (INT16) IANAcharset,
      (WCHAR**) pCallURL, (BOOL) isTopPriority ) );
}


void sdl2c_AbortScriptsForSpecificUA( void* thisx, unsigned long UApidAsInt )
{
	SQ_AbortScriptsForSpecificUA( (pstructSQ) thisx, (UINT32) UApidAsInt );
}



long sdl2c_Exec( void* thisx, long* pInvokeIDThatExecuted, unsigned long* pUApidAsInt )
{
	UINT8 l_pInvokeIDThatExecuted;
	UINT32 l_pUApidAsInt;

	long result = (long) SQ_Exec( (pstructSQ) thisx, & l_pInvokeIDThatExecuted, & l_pUApidAsInt );

	*pInvokeIDThatExecuted = l_pInvokeIDThatExecuted;
	*pUApidAsInt = l_pUApidAsInt;

	return result;
}



long sdl2c_GetResult( void* thisx, long invokeID, void **pResultStr )
{
	return( (long)SQ_GetResult( (pstructSQ) thisx, (UINT8) invokeID, (WCHAR**) pResultStr ) );
}


long sdl2c_GetGoParams( void* thisx, long invokeID, void **pGoUrl )
{
	return( (long)SQ_GetGoParams( (pstructSQ) thisx, (UINT8) invokeID, (BYTE**) pGoUrl ));
}


long sdl2c_GetPrevParams( void* thisx, long invokeID )
{
	return( (long) SQ_GetPrevParams( (pstructSQ) thisx, (UINT8) invokeID ));
}


long sdl2c_GetError( void* thisx, long invokeID, long *errCode )
{
	enumErrorCode l_errCode;
	
	long result = (long)SQ_GetError( (pstructSQ) thisx, (UINT8) invokeID, & l_errCode );

	*errCode = l_errCode;

	return result;
}


long sdl2c_GetExternalCallParams( void* thisx, long invokeID, void **pTheURL )
{
	return( (long)SQ_GetExternalCallParams( (pstructSQ) thisx, (UINT8) invokeID, 
																					(BYTE**) pTheURL ) );
}


long sdl2c_GetLibCallParams( void* thisx, long invokeID, unsigned long *pLibIndex, long *pFuncIndex, 
															void **pArgOpS )
{
	UINT16 l_pLibIndex;
	UINT8 l_pFuncIndex;

	long result = (long)SQ_GetLibCallParams( (pstructSQ) thisx, (UINT8) invokeID, & l_pLibIndex, 
															& l_pFuncIndex, (pstructOpS*) pArgOpS );

	*pFuncIndex = l_pFuncIndex;
	*pLibIndex = l_pLibIndex;

	return result;
}


long sdl2c_GetAnArgumentAsString( void* argOpS, void** pArgAsStr )
{
	return( (long)SQ_GetAnArgumentAsString( (pstructOpS) argOpS, (WCHAR**) pArgAsStr ) );
}


void sdl2c_ReturnValueToScript( void* thisx, long invokeID, void** theValueStr )
{
	SQ_ReturnValueToScript( (pstructSQ) thisx, (UINT8) invokeID, (WCHAR**) theValueStr );
}


void sdl2c_LoadErrorReceived( void* thisx, long invokeID )
{
	SQ_LoadErrorReceived( (pstructSQ) thisx, (UINT8) invokeID );
}


void sdl2c_LoadDoneReceived( void* thisx, long invokeID, void** pBP, unsigned long BPlen, long IANAcharset )
{
	SQ_LoadDoneReceived( (pstructSQ) thisx, (UINT8) invokeID, (BYTE**) pBP, (UINT32) BPlen, (INT16) IANAcharset );
}


void sdl2c_LibraryErrorReceived( void* thisx, long invokeID )
{
	SQ_LibraryErrorReceived( (pstructSQ) thisx, (UINT8) invokeID );
}


void sdl2c_ReturnVarValueToScript( void* thisx, long invokeId, void** ppResultVar )
{
	SQ_ReturnVarValueToScript( (pstructSQ) thisx, (UINT8)invokeId, (pstructVar*)ppResultVar );
}


void sdl2c_ReturnStringValueToScript( void* thisx, long invokeID, void** pValueStr )
{
	SQ_ReturnStringValueToScript( (pstructSQ) thisx, (UINT8) invokeID, (WCHAR**) pValueStr );
}


void sdl2c_ReturnBoolValueToScript( void* thisx, long invokeID, long theValue )
{
	SQ_ReturnBoolValueToScript( (pstructSQ) thisx, (UINT8) invokeID, (BOOL) theValue );
}



void sdl2c_ReturnIntValueToScript( void* thisx, long invokeID, long theValue )
{
	SQ_ReturnIntValueToScript( (pstructSQ) thisx, (UINT8) invokeID, (INT32) theValue );
}


void sdl2c_LoadStringDone( void* thisx, long invokeID, long charSet, void** pValueStr, unsigned long strLen )
{
	SQ_LoadStringDone( (pstructSQ)thisx, (UINT8)invokeID, (INT16)charSet, (BYTE**)pValueStr, (UINT32)strLen );
}


long sdl2c_GetViewId( void* thisx, long invokeID, long* pResult )
{
	UINT8 l_pResult;

	long result = (long) SQ_GetViewId( (pstructSQ) thisx, (UINT8) invokeID, & l_pResult );

	*pResult = l_pResult;

	return result;
}


void sdl2c_DeleteArgOpS( void** pArgOpS )
{
	SQ_DeleteArgOpS( (pstructOpS*) pArgOpS );
}


void sdl2c_Var_Delete( void** ppVar)
{
	Var_Delete( (pstructVar*) ppVar );
}


#ifdef CAN_SIGN_TEXT
void sdl2c_TextSigned( void* thisx, long invokeId, long algoritm, long sigLen, void* signature, void* hashedKey, long hashedKeyLen, void* certificate, long certificateLen, long certificateType, long err)
{
	SQ_TextSigned( (pstructSQ)thisx, (UINT8)invokeId, (UINT8)algoritm, (UINT16)sigLen, (CHAR*)signature, (CHAR*)hashedKey, (UINT16)hashedKeyLen, (CHAR*)certificate, (UINT16)certificateLen, (UINT8)certificateType, (UINT16)err);
}
#endif

#ifdef USE_PROPRIETARY_WMLS_LIBS

void sdl2c_ProprietaryLibraryFunctionResponse( void* thisx, long invokeID, void** pVar )
{
/* Modified by GBU,000322
	SQ_ProprietaryLibraryFunctionResponse( (pstructSQ)thisx, (UINT8) invokeID, (WMLSvar**)pResultVar );
*/
	SQ_ProprietaryLibraryFunctionResponse( (pstructSQ)thisx, (UINT8) invokeID, (WMLSvar**)pVar );
}


void sdl2c_WMLSvarDelete( void** pVar, long externallyAllocated )
{
	WMLSvar_Delete( (WMLSvar**) pVar, (BOOL) externallyAllocated );
}

#endif
