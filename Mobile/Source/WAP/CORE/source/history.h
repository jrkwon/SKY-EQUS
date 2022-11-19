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
/*========================================================================
	FILE: History.h
	
	General History handler for WAE

	Rev history:
		991124	JPR		Created, functions moved from WMLIF.h
		991124	FAR		A generic history functionality model added

=========================================================================*/

#ifndef CHISTORY_H
#define CHISTORY_H

#include "cmmnrsrc.h"
#include "tapicmmn.h"
#include "wmldef.h"



/*========================================================================

	HISTORY LIST FUNCTIONS

	Functions used for the history list handling. 

=========================================================================*/
/*---------------------------------- generic History functions, ------------------------------------------*/

void Delete_HistoryItem(void ** ppHistoryList);

HISTORYITEM* New_HistoryItem(void);

HISTORYITEM* CreateAndInitNewHistoryItem(BYTE *pbURLEntry, WCHAR *pchItemTitle, BYTE* pbPostData, UINT16 iPostDataLength, UINT8 iMethod, UINT8 iContentType, INT16 iContentCharSet);
/*	EmptyHistoryList  (clears the list from ppHistoryList) */
void EmptyHistoryList (HISTORYITEM ** ppHistoryList);

HISTORYITEM* HistoryRead (HISTORYITEM* pHistoryList, int iPosition);

/* pops the history stack (deletes the element) and returns the new topmost element */
void HistoryListPop (HISTORYITEM** ppHistoryList);

HISTORYITEM* HistoryListPush(HISTORYITEM* pHistoryList, HISTORYITEM* pNewHistoryItem, UINT16 iMax );

/* 	HistoryDeleteLeastUsed returns the number of elements available in the history */
UINT16 DeleteLastHistoryItems(HISTORYITEM* pHistoryList, UINT16 iMax);

UINT16 GetHistoryTotal(HISTORYITEM* pHistoryList);

/*---------------------------------- WML specific History functions, ------------------------------------------*/


/*========================================================================
	WML_EmptyHistoryList
==========================================================================
	Clears the History list in the specified User Agent.
	
	Input: Actual User Agent
	Output: -
==========================================================================*/

void WML_EmptyHistoryList (pUA pUser);



/*========================================================================
	WML_EmptyHistoryNewContext
==========================================================================
	Removes all entries, but the top entry, in the History list in the 
	specified user Agent.

	Input: Actual User Agent
	Output: -
==========================================================================*/
void WML_EmptyHistoryNewContext (pUA pUser);


/*========================================================================
	WML_HistoryRead
==========================================================================
	Returns the a COPY of the URI at the specified position.

	Input: Actual User Agent and position
	Output: BYTE*, NULL if not found.
==========================================================================*/
BYTE* WML_HistoryRead (pUA pUser, int iPosition);


/*========================================================================
	WML_HistoryListPush
==========================================================================
	Adds a entry on the history list in the specified User Agent. Stores 
	COPIES of the strings. If the number of elements exceeds the maximum 
	allowed	number, the least used element is removed.

	Input: Actual User Agent, entry (BYTE*) and the title of the entry 
		   (WCHAR*)
	Output: TRUE if OK, FALSE otherwise.
==========================================================================*/
BOOL WML_HistoryListPush (pUA pUser, BYTE *pbURLEntry, WCHAR *pchItemTitle , HISTORYITEM* pHistoryItem);

/*========================================================================
	WML_HistoryDeleteLeastUsed
==========================================================================
	Deletes the least used item in the history list.

	Input: Actual User Agent
	Output: -
==========================================================================*/
void WML_HistoryDeleteLeastUsed (pUA pUser);


/*========================================================================
	WML_SetMaxInHistoryList
==========================================================================
	Sets the maximum number of allowed elements in the history list. (0 
	indicates unlimited.) When the number is changed, the new list is 
	updated to contain no more than the maximum number of allowed elements.

	Input: Actual User Agent, UINT16
	Output: -
==========================================================================*/
void WML_SetMaxInHistoryList (pUA pUser, UINT16 iMaxElements);


/*========================================================================
	WML_GetHistoryListTitles
==========================================================================
	Returns a array of pointers to the titles in the history list. 
	
	NOTE! The titles are NOT copied, do not deallocate!

	Input: Actual User Agent
	Output: Pointer to WCHAR*, NULL if error.
==========================================================================*/
WCHAR** WML_GetHistoryListTitles (pUA pUser);

void WML_HistoryListPop ( UA* pUser);

void SDL_PutInHistory(void* pUser, void* pInUrl, void* pCurrentCard, void* pHistoryItem);

void SDL_GetPreviousCardURL(void* pUser, void** ppURL, void** ppHistoryItem );

#endif
