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
#ifndef WAE_CMMN_H
#define WAE_CMMN_H

#include "tapicmmn.h"
#include "capiclnt.h"
#include "WAEDef.h"
#include "WMLIF.h"
#include "config.h"

#ifdef XSCT_CMICRO
#include "ml_typ.h"
#else
#include "scttypes.h"
#endif


typedef struct
{
	void* pData;		/* can be any associated data */
	unsigned long int uiPid;
} UAPIDSTRUCT, *pUAPIDSTRUCT;
 
/*
 * Convert a SDL PId to an integer.
 */
UINT32  PId2Int (SDL_PId pid);

/*
 * Convert an integer to a SDL PId.
 */
SDL_PId Int2PId (UINT32 n);

int UAAddNewPid( void* pList, int iId, unsigned long int uiPid , void* pTheUA);
void* GetUAInList( void* pList, unsigned long int uiPid);
unsigned long int UAGetPid( void* pList, int iId, int iRemove );
int UARemovePid( void* pList, unsigned long int uiPid );



void SDL_CallFunction( int iTID, void** pRequestStruct );

void SDL_DeleteParameters(void** pppInParameters);
void SDL_StoreParameters(void* pUser, void** pppInParameters);

void SDL_InitializeGlobalVars(void* WAEManagementObject);
BOOL SDL_InitializeUAVars(int ViewID, void* pInUserAgent, void* pUAParameters);
int CopyCredentialParameters (void* pUserName, void* pPassWord, void** ppCredentials);

void* SDL_CreateNewUASettings(void* pUserAgent, void* pUAParameters);

void SDL_DeleteAllConfig(void);

void* SDL_CreateNewNetworkParameters(void* pUserAgent);

BOOL    UA_ConfigStr( UAPARAM* pUAParam, int ConfigKind, ConfigValue, ConfigSize);

/*--------------------------------------------------------
                   Listhandling
---------------------------------------------------------*/
/*-------------------------------------------------
Creates a new list with a maximum of iMax elements
--------------------------------------------------*/
void* SDL_NewList(int iMax);


/*-------------------------------------------------
clears all the listelements in the list, NOT the
actual elements pointed to, these must be deallocated
separately. Total elements is set to 0. The listentry 
saved intact.
--------------------------------------------------*/
void SDL_ClearList(void* pTheList);


/*-------------------------------------------------
clears the listelements and deletes the list
To be used when a COMPLETE removal of
the list is wanted
--------------------------------------------------*/
void SDL_DeleteList(void** ppTheList);

 
/*-------------------------------------------------
The pRElement is deleted from the list - NOT
the element pointed to
--------------------------------------------------*/
void SDL_DeleteListElement(void* pTheList, void* pRElement);


/*-------------------------------------------------
Gets element with id iID, start searching from the
pSearchFrom listelement, if pSearchFrom is NULL then 
the search is performed from the beginning.  
If a matching element is found this is returned 
otherwise NULL.
--------------------------------------------------*/
void* SDL_SearchListElement(void* pTheList, int iId, void* pSearchFrom);


/*-------------------------------------------------
Gets next LISTelement. 
If pTheReqElm is NULL then the first element is returned (like a GetFirstElement)
--------------------------------------------------*/
void* SDL_GetNextListElement(void* pTheList,void* pTheReqElm);


/*-------------------------------------------------
Gets the content of a LISTElement i.e the element
pointed to.
--------------------------------------------------*/
void* SDL_GetListElementContent(void* pTheReqElm);


/* --------------------------------------------------
 * Change the content of a LISTElement, i.e., the generic
 * data pointed to.
 * -------------------------------------------------- */
void
SDL_ChangeListElementContent (void* pTheReqElm, void* pNewContentElement);


/*-------------------------------------------------
Gets the id of a LISTElement 
--------------------------------------------------*/
int SDL_GetListElementID( void* pTheReqElm);

/* --------------------------------------------------
 * Change the id of a LISTElement.
 * -------------------------------------------------- */
void SDL_ChangeListElementID (void* pTheReqElm, int iId );

/*-------------------------------------------------
Add a new element to the list

puts pElement in the list and returns 1 (TRUE) if OK


if iPrio != 0 then the list is a priority list, where
a iPrio controls the priority position. 
--------------------------------------------------*/
int SDL_AddListElement(void* pList, int iId, int iPrio ,void* pElement);


/*-------------------------------------------------
Gets number of element in list 
--------------------------------------------------*/
int SDL_GetCardinal(void* pList);

/*-------------------------------------------------
Add a new element first in the list returns 1 (TRUE) if OK
--------------------------------------------------*/
int SDL_AddElementFirst(void* pList, int iId, void* pElement);

/*-------------------------------------------------
Add a new element after the specified in the list returns 1 (TRUE) if OK
--------------------------------------------------*/
int SDL_AddElementAfter(void* pList, int iId, void* pElement, void* pAfter);

/*-------------------------------------------------
Add a new element last in the list returns 1 (TRUE) if OK
--------------------------------------------------*/
int SDL_AddElementLast(void* pList, int iId, void* pElement);



/*--------------------------------------------------------
                 Stackfunctions
---------------------------------------------------------*/

void* SDL_NewLIFOStack(void);

void SDL_ClearLIFOStack( void** ppStack );

void* SDL_POPLIFOStack(void* pStack);

int SDL_PUSHLIFOStack( void* pStack, void* pElement );

void* SDL_CheckLIFOStack( void *pStack );


/*---------------------------------------------------------
                  TID MANAGEMENT
----------------------------------------------------------*/

/*---------------------------------------------
Returns a new TID from the TID pool, 0 indicates an error and
is NOT a valid TID
----------------------------------------------*/
int SDL_CreateNewGlobalTID(void* pWAEInObj, int iViewID);

/* ---------------------------------------------
 * Return the next active TID strictly larger
 * than iPrevTID. If iPrevTID is 0, the smallest
 * active TID is returned.
 * On error, or if there is no active TID larger
 * than iPrevTID, this routine returns 0.
 * ----------------------------------------------*/
int SDL_GetNextActiveTID (void* pWAEInObj, int iPrevTID);

/*---------------------------------------------
Deactivate a TID in the TID pool - i.e the 
corresponding request is no longer active.
----------------------------------------------*/
void SDL_DeactivateGlobalTID(void* pWAEInObj, int iTID);

/*---------------------------------------------
Deactivate a ALL TIDs in the TID pool - i.e the 
corresponding request is no longer active.
----------------------------------------------*/
void SDL_DeactivateAllGlobalTIDs(void* pWAEInObj);

/*---------------------------------------------
Check a TID in the TID pool - is the 
corresponding request active. Returns 0 (FALSE) if not active
----------------------------------------------*/
int SDL_CheckGlobalTIDStatus(void* pWAEInObj, int iTID);

/*---------------------------------------------
Releases a TID from the TID pool
----------------------------------------------*/
void SDL_ReleaseGlobalTID(void* pWAEInObj, int iTID);          

/*---------------------------------------------
Enables SDL to dynamically free structures in C
e.g. strings
Input: Datapointer
Output: -
----------------------------------------------*/
void SDL_Free(void** pTheData);

/*---------------------------------------------
Enables SDL to output errormessages to the application
Input: errNo , defined in the porting guide.
Output:
----------------------------------------------*/

void SDL_OutputError(void* pUser, int iErrorcode, int iErrType);

void SDL_OutputStatus(void* pUser, int iStatus, void* pURL );

void SDL_LOG(void* pUser, int logno, void* pString);

void* GenCopyString(INT8 iSize, const void* pStr);


#endif
