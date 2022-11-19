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
#ifndef _PushLL_H
#define _PushLL_H
/*========================================================================

	WAP Implementation Project

==========================================================================

	File: PushLL.h

	Description:

	Author: Martin Ranstorp, AU-System AB

	Revision history:
  	Date    Rev  Sign  Comment
  	990930  PA1  MRP   First version
	991206  PA2  MRP   Updated. Working in WAE test environment
	000124  PA3  MRP   Updated after inspection.
	000214  PA4  MRP   Copyright added.
	000316	PA5	 MRP   Push API changed.
	001212  PA6  HEAD  The repository can now be opened and closed.

=========================================================================*/

#include "cmmnrsrc.h"
#include "push.h"
#include "wae_cmmn.h"
#include "storage.h"


/***********************/
/* Debug stuff         */
/***********************/

#ifdef WAE_DEBUG
    #if defined XSCT_CMICRO || defined NOT_USING_SDL
      #define URL_DEBUG_PRINT(a)         printf(a)
    #else
      #define URL_DEBUG_PRINT(a)         xTraceAlg(a)
    #endif
#endif


/*========================================================================
	DEFINITIONS
=========================================================================*/

typedef struct PUSHCONTEXT
{
	/* Used to init the push buffer. Used for all content in PUSH_ScanPushBuffer */
	UINT16	iNumberOfElements;
	UINT16	iCurrentElement;
	UINT32*	piElementList;
	UINT8 iScanPhase;

	/* Storage object */
	StorageObject Storage;

} PUSHCONTEXT, *pPUSHCONTEXT;



/*========================================================================
	FUNCTION PROTOTYPES
=========================================================================*/

/*========================================================================
	PUSH_Store
==========================================================================
Purpose: Assist function to Store an Push in permanent push memory
Params:  vsPushContent (MUST be of pPUSHCONTENTSTRUCT type)
Return:  0 = OK, -1 = Error, -2 = Message larger than repository.
=========================================================================*/
INT8 PUSH_Store( void* pContext, void* pvPushContent, 
					UINT32* piContentID );

/*========================================================================
	PUSH_StorePush
==========================================================================
Purpose: Store the Push in memory
Params:  pContext, piContentID
Return:  0 = OK, -1 = Error, -2 = Message larger than repository.
=========================================================================*/
INT8 PUSH_StorePush( void* pContext, void* pvContent,
					UINT32* piContentID );

/*========================================================================
	PUSH_DeletePush
==========================================================================
Purpose: Delete the Push content
Params:  pContext, iContentID
Return:  TRUE
=========================================================================*/
BOOL PUSH_DeletePush( void* pContext, UINT32 iContentID, UINT8 iType );

/*========================================================================
	PUSH_GetStoredSI
==========================================================================
	The function extracts data from the instream and returns the SI
	part of a PUSHCONTENTSTRUCT. The instream must be correct. It is the
	caller's responsibility to deallocate the returned data.

	The function is called by PUSH_GetContentWithId

	Input: Data
	Output: Pointer to SICONTENTSTRUCT
==========================================================================*/
pSICONTENTSTRUCT PUSH_GetStoredSI (BYTE* pbData);

/*========================================================================
	PUSH_GetStoredSL
==========================================================================
	The function extracts data from the instream and returns the SL
	part of a PUSHCONTENTSTRUCT. The instream must be correct. It is the
	caller's responsibility to deallocate the returned data.

	The function is called by PUSH_GetContentWithId

	Input: Data
	Output: Pointer to SLCONTENTSTRUCT
==========================================================================*/
pSLCONTENTSTRUCT PUSH_GetStoredSL (BYTE* pbData);

/*========================================================================
	PUSH_ScanForContent
==========================================================================
	The function scans the memory for content and creates a list of all 
	pushes in the push buffer (pushes that have expired are removed from
	the push buffer). The list is of the type LISTHEADER and the elements 
	in the list are of the type PUSHLISTSTRUCT. It is the caller's 
	responsibility to deallocate the memory. The function should be 
	called repeatedly until it returns FALSE. 

	NOTE! When the function is called the first time, ppContentList 
	should be NULL.

	Input: void* (MUST be pPUSHCONTEXT), void** (MUST be LISTHEADER**)
	Output: TRUE if the function should be called again, FALSE otherwise
==========================================================================*/
BOOL PUSH_ScanForContent ( void* pContentList, void* pContext );

/*========================================================================
	PUSH_GetContentWithID
==========================================================================
Purpose: Fetch PushContent with ContentID
Params:  iContentID
Return:  pPushContentStruct
=========================================================================*/
void* PUSH_GetContentWithID( void* pContext, UINT32 iContentID,
										 UINT8 iType);

/*========================================================================
	PUSH_ChangePushStatus
==========================================================================
Purpose: Change status on PushContent with ContentID
Params:  iContentID
Return:  TRUE
=========================================================================*/
BOOL PUSH_ChangePushStatus (void* pContext, UINT32 iContentID, UINT8 iStatus);

/*========================================================================
	PUSH_CompactPushBuffer
==========================================================================
	This function organizes the memory with no (minimal) fragmentation. 
	The function is called when a compactation of the push buffer is 
	needed. The function is called, for example, when content is to be 
	stored and the available memory in the push buffer is sufficient, but 
	fragmented in a way so that the largest free block is smaller than 
	the content. To allow scheduling, the compact process is not neces-
	sarily completed with one function call. As long as the function 
	returns TRUE it is possible to compact the push buffer more.

	Input: void* (MUST be pPUSHCONTEXT)
	Output: TRUE if possible to compact more, FALSE otherwise
==========================================================================*/
BOOL PUSH_CompactPushBuffer (void* pContext);

/*========================================================================
	PUSH_InitPushMem
==========================================================================
	The function creates a PUSHCONTEXT struct and initiates the fields in 
	the struct. NULL is returned if error.
	
	Input: Size of push memory.
	Output: void* (type is pPUSHCONTEXT), NULL if error
==========================================================================*/
void* PUSH_InitPushMem (UINT32 iPushSize);

/*========================================================================
	PUSH_ResetPushMem
==========================================================================
	The function removes all content in the push memory.

	Input: void* (MUST be pPUSHCONTEXT)
	Output: -
==========================================================================*/
void PUSH_ResetPushMem (void* pContext);

/*========================================================================
	PUSH_TerminatePushMem
==========================================================================
	The function removes the PUSHCONTEXT struct.
	
	Input: void* (MUST be pPUSHCONTEXT)
	Output: -
==========================================================================*/
void PUSH_TerminatePushMem (void* pContext);

/*========================================================================
	PUSH_ClosePushMem
==========================================================================
	The function closes the pushMem. Run PUSH_InitPushMem to be able to use the 
	memory again. If more data is stored than liNewSize specifies then data
	will be removed.
	
	Input: pContent MUST be pPUSHCONTEXT
		   pContentList must be of type ListHeader
		   liNewSize (How much can be saved)
		   iType (if content should be removed then -1=oldest SI, -2 =oldest SL)
	Output: TRUE if closed, false if more content should be removed.
==========================================================================*/
BOOL PUSH_ClosePushMem(void * pContent, void *pContentList, UINT32 liNewSize, INT8 iType);

#endif
