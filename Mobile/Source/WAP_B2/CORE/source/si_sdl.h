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
#ifndef _SI_SDL_H
#define _SI_SDL_H


void* sdl2c_Init( void );
void sdl2c_Terminate( void **pThis );

long sdl2c_InvokeScript( void* thisx, unsigned long UApidAsInt, void* theUA, void** pBP,
		unsigned long BPlen, long IANAcharset, void** pCallURL, long isTopPriority );
void sdl2c_AbortScriptsForSpecificUA( void* thisx, unsigned long UApidAsInt );

long sdl2c_Exec( void* thisx, long* pInvokeIDThatExecuted, unsigned long* pUApidAsInt );

long sdl2c_GetResult( void* thisx, long invokeID, void **pResultStr );
long sdl2c_GetGoParams( void* thisx, long invokeID, void **pGoUrl );
long sdl2c_GetPrevParams( void* thisx, long invokeID );
long sdl2c_GetError( void* thisx, long invokeID, long *errCode );
long sdl2c_GetExternalCallParams( void* thisx, long invokeID, void **pTheURL );
long sdl2c_GetLibCallParams( void* thisx, long invokeID, unsigned long *pLibIndex, long *pFuncIndex, 
															void **pArgOpS );
void sdl2c_LoadErrorReceived( void* thisx, long invokeID );
void sdl2c_LoadDoneReceived( void* thisx, long invokeID, void** pBP,
		unsigned long BPlen, long IANAcharset );

void sdl2c_LibraryErrorReceived( void* thisx, long invokeID );

void sdl2c_ReturnValueToScript( void* thisx, long invokeID, void** theValueStr );
void sdl2c_ReturnVarValueToScript( void* thisx, long invokeId, void** ppResultVar );
void sdl2c_ReturnStringValueToScript( void* thisx, long invokeID, void** pValueStr );
void sdl2c_ReturnBoolValueToScript( void* thisx, long invokeID, long theValue );
void sdl2c_ReturnIntValueToScript( void* thisx, long invokeID, long theValue );

void sdl2c_LoadStringDone( void* thisx, long invokeID, long charSet, void** pValueStr, unsigned long strLen );

long sdl2c_GetViewId( void* thisx, long invokeID, long* pResult );

long sdl2c_GetAnArgumentAsString( void* argOpS, void** pArgAsStr );
void sdl2c_DeleteArgOpS( void** pArgOpS );

void sdl2c_Var_Delete( void** ppVar);

#ifdef USE_PROPRIETARY_WMLS_LIBS
void sdl2c_ProprietaryLibraryFunctionResponse( void* thisx, long invokeID, void** pVar );
void sdl2c_WMLSvarDelete( void** pVar, long externallyAllocated );
#endif

#endif
