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
#ifndef _SI_SQ_H
#define _SI_SQ_H


#include "cmmnrsrc.h"
#include "si_si.h"
#include "WMLDef.h"

#ifdef USE_PROPRIETARY_WMLS_LIBS
	#include "aapiclnt.h"
#endif


typedef struct BPElement {
	BYTE									*BP;
	UINT32								BPlen;
	INT16									IANAcharset;				/* header field from WSP for the BP file. Is used to determine string encoding of WMLS strings in the BP file */
	WCHAR									*callURL;
	BOOL									isInitialBPInvoked;	/*	TRUE: the callURL includes funcName and args. 
																								FALSE the funcName and args can be fetched from next SI  */
	pstructSI							theSI;
	struct BPElement			*next;

} structBPElement, *pstructBPElement;



typedef struct InvokeHead {
	UINT8									invokeID;
/* SDL pids are stored in SDL and this is only an index into an array of pids to refer to the correct UA process  */
	UINT32								UApidAsInt;	
	pUA										theUAStruct;		/* must NOT be deallocated when InvokeHead is deleted */
	BOOL									isDone;					/* TRUE: this invoke thread is done adn is waiting for someone to collect the result 
																							FALSE: not done */
	BOOL									isInWaitState;	/* TRUE if waiting for external resources. FALSE: if ready to run */
	enumErrorCode							errorCode;			/* if error occurs the whole thread is deallocated and the errCode is stored here */
	pstructVar						theResult;			/* when the script is done, the call list is  */
	BOOL									browserPrev;
	WCHAR									*browserGoUrl;
	pstructBPElement			BPcallList;
	struct InvokeHead			*next;

} structInvokeHead, *pstructInvokeHead;


typedef struct SQ {
	UINT16								timeSlice;
	UINT8									nextInvokeID;				/* this is so that one knows which invokeID to give to the next Invoke */
	BOOL									doRoundRobinScheduling;
	BOOL									onlyOneScriptPerUA;
	BOOL									handlesTopPriorityREQ;	/* if handles top priority requests */
	pstructInvokeHead			currentInvoke;					/* steps through the InvokeHead elements if round robin scheduling is used */
	pstructInvokeHead			invokeList;

} structSQ, *pstructSQ;


typedef enum {
	eSs_more = 1,
	eSs_done,
	eSs_done_prev,	/* done and the UA should perform a PREV action */
	eSS_done_go,		/* done and the UA should perform a GO action with a url found in si->goURL */
	eSs_error,
	eSs_extFunc,
	eSs_lib
} enumScriptStatus;


pstructSQ SQ_Init( UINT16 timeSlice, BOOL roundRobin, BOOL oneScriptPerUA, BOOL handlesTopPrio );
VOID SQ_Terminate( pstructSQ *pThis );

UINT8 SQ_InvokeScript( pstructSQ thisx, UINT32 UApidInt, pUA theUA, BYTE **pBP, UINT32 BPlen, INT16 IANAcharset, WCHAR **pCallURL, 
											 BOOL isTopPriority );
VOID SQ_AbortScriptsForSpecificUA( pstructSQ thisx, UINT32 UApidInt );

enumScriptStatus SQ_Exec( pstructSQ thisx, UINT8 *pInvokeIDThatExecuted, UINT32 *pUApidInt );

BOOL SQ_GetResult( pstructSQ thisx, UINT8 invokeID,WCHAR **pResultStr );
BOOL SQ_GetGoParams( pstructSQ thisx, UINT8 invokeID, BYTE **pGoUrl );
BOOL SQ_GetPrevParams( pstructSQ thisx, UINT8 invokeID );
BOOL SQ_GetError( pstructSQ thisx, UINT8 invokeID, enumErrorCode *errCode );
BOOL SQ_GetExternalCallParams( pstructSQ thisx, UINT8 invokeID, BYTE **pTheURL );
BOOL SQ_GetLibCallParams( pstructSQ thisx, UINT8 invokeID, UINT16 *pLibIndex, UINT8 *pFuncIndex, 
															pstructOpS *pArgOpS );

VOID SQ_ReturnValueToScript( pstructSQ thisx, UINT8 invokeID, WCHAR **theValueStr );
VOID SQ_ReturnVarValueToScript( pstructSQ thisx, UINT8 invokeID, pstructVar *ppResultVar );
VOID SQ_ReturnStringValueToScript( pstructSQ thisx, UINT8 invokeID, WCHAR **theValue );
VOID SQ_ReturnBoolValueToScript( pstructSQ thisx, UINT8 invokeID, BOOL theValue );
VOID SQ_ReturnIntValueToScript( pstructSQ thisx, UINT8 invokeID, INT32 theValue );

VOID SQ_LoadStringDone( pstructSQ thisx, UINT8 invokeID, INT16 charSet, BYTE **pValueStr, UINT32 strLen );
/* LoadStringError returns an integer error code and thus SQ_ReturnIntValueToScript must be used */

VOID SQ_LibraryErrorReceived( pstructSQ thisx, UINT8 invokeID );

BOOL SQ_GetViewId( pstructSQ thisx, UINT8 invokeID, UINT8 *pResult );

VOID SQ_LoadErrorReceived( pstructSQ thisx, UINT8 invokeID );
VOID SQ_LoadDoneReceived( pstructSQ thisx, UINT8 invokeID, BYTE **pBP, UINT32 BPlen, INT16 IANAcharset );

BOOL SQ_GetAnArgumentAsString( pstructOpS argOpS, WCHAR **pArgAsStr );
VOID SQ_DeleteArgOpS( pstructOpS *pArgOpS );

#ifdef USE_PROPRIETARY_WMLS_LIBS
VOID SQ_ProprietaryLibraryFunctionResponse( pstructSQ thisx, UINT8 invokeID, WMLSvar** pResultVar );
#endif



#endif
