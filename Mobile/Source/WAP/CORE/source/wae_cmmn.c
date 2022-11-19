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


#ifndef WAE_CMMN_C
#define WAE_CMMN_C

#include "waedef.h"
#include "wae_cmmn.h"
#include "wmldef.h"
#include "aapiclnt.h"
#include "aapimmi.h"
#include "url.h"

/* ----------------------------------------------
 *               LISTHANDLING STRUCTS
 * ----------------------------------------------
 */

/* Each element in the list: */
typedef struct tLISTELEMENT
{
  int iIdentifier;                 /* Arbitrary identifier - optional */
  int iPrio;                       /* Priority info */
  void* pTheElement;               /* Generic data */
  struct tLISTELEMENT *pNext;      /* Doubly linked, more efficient. */
  struct tLISTELEMENT *pPrev;
} LISTELEMENT, *pLISTELEMENT;

/* The list header */
typedef struct
{
  int iMax;                        /* 0 = no upper limit */
  int iTotal;                      /* Number of elements currently in list */
  LISTELEMENT head;                /* A list element used as header node. */
} LISTHEADER, *pLISTHEADER;


typedef struct
{
   UINT8 iViewID;
   BOOL iActive;
} UAREQSTATUS,*pUAREQSTATUS;

/* ---------------------------------------------------------------------------------------------------------

                                            PID mgmt

 -------------------------------------------------------------------------------------------------------- */

/*
 * Convert a SDL PId to an integer.
 */
UINT32
PId2Int (SDL_PId pid)
{
#ifdef XSCT_CMICRO
  return (UINT32)pid;
#else
  return (UINT32)(pid.LocalPId);
#endif
}

/*
 * Convert an integer to a SDL PId.
 */
SDL_PId
Int2PId (UINT32 n)
{
#ifdef XSCT_CMICRO
  return (SDL_PId)n;
#else
  SDL_PId pid;

  pid.LocalPId = (xLocalPIdNode)n;
  pid.GlobalNodeNr = xGlobalNodeNumber ();

  return pid;
#endif
}

/* add a new pid to a list , NOTE: the pid must be converted first */
int UAAddNewPid( void* pList, int iId, unsigned long int uiPid , void* pTheUA)
{
	UAPIDSTRUCT* pPidStruct = NULL;
	if( pList != NULL ){
		pPidStruct = NEWSTRUCT(UAPIDSTRUCT);

		if( pPidStruct != NULL ){
			pPidStruct->uiPid = uiPid;
			pPidStruct->pData = pTheUA;
			return SDL_AddListElement( pList, iId, 0, (void*)pPidStruct );

		}
	}
	return 0;
	
} /* end of pidList_AddPid */

void* GetUAInList( void* pList, unsigned long int uiPid)
{
	void*		pvListElement = NULL;
	void*		pvElementContent = NULL;
	UAPIDSTRUCT*	pPidStruct = NULL;

	if( pList == NULL )
	return NULL;

	pvListElement = SDL_GetNextListElement( pList, NULL );
	while( pvListElement != NULL ){
		pvElementContent = SDL_GetListElementContent( pvListElement );
		if( pvElementContent != NULL ){
			pPidStruct = (UAPIDSTRUCT*)pvElementContent;
			if( pPidStruct && pPidStruct->uiPid == uiPid )
			{
				return pPidStruct->pData;
			}
		}
		pvListElement = SDL_GetNextListElement( pList, pvListElement );
	}
	
	return NULL;
 
} /*  */



unsigned long int UAGetPid( void* pList, int iId, int iRemove )
{
	void*		pvListElement = NULL;
	void*		pvElementContent = NULL;
	unsigned long int	uiPID = 0;
	UAPIDSTRUCT*	pPidStruct = NULL;

	if( pList != NULL ){
		pvListElement = SDL_SearchListElement( pList, iId, NULL );
		if( pvListElement != NULL ){
			pvElementContent = SDL_GetListElementContent( pvListElement );
			if( pvElementContent != NULL ){
				pPidStruct = (UAPIDSTRUCT*)pvElementContent;
				if( pPidStruct != NULL ){
					if( iRemove ){
						uiPID = pPidStruct->uiPid;
						DEALLOC( &pPidStruct );
						SDL_DeleteListElement( pList, pvListElement );
						return uiPID;
					}
					else
						return pPidStruct->uiPid;
				}
			}
		}
	}
	return 0;
	
} /* end of pidList_GetPid */



int UARemovePid( void* pList, unsigned long int uiPid )
{
	void*		pvListElement = NULL;
	void*		pvElementContent = NULL;
	UAPIDSTRUCT*	pPidStruct = NULL;

	if( pList != NULL ){
		pvListElement = SDL_GetNextListElement( pList, NULL );
		while( pvListElement != NULL ){
			pvElementContent = SDL_GetListElementContent( pvListElement );
			if( pvElementContent != NULL ){
				pPidStruct = (UAPIDSTRUCT*)pvElementContent;
				if( pPidStruct != NULL && pPidStruct->uiPid == uiPid ){
					DEALLOC( &pPidStruct );
					SDL_DeleteListElement( pList, pvListElement );
					return 1;
				}
			}
			pvListElement = SDL_GetNextListElement( pList, pvListElement );
		}
	}
	return 0;
	
} /* end of pidList_RemovePid */


/*--------------------------------------------------------------------------------------------------------

										Variable handling (setting etc)

--------------------------------------------------------------------------------------------------------*/

void SDL_DeleteParameters(void** pppInParameters)
{
   variableType** ppTempParameters; 
   ppTempParameters = (variableType**)*pppInParameters;
   if (ppTempParameters)
   {
	   while (*ppTempParameters)
	   {
		  DEALLOC(& (*ppTempParameters)->value );
		  DEALLOC(& (*ppTempParameters)->name );
		  DEALLOC( ppTempParameters ); 
		  ppTempParameters++;
	   }
	   DEALLOC(pppInParameters);
   }
}

void SDL_StoreParameters(void* pUser, void** pppInParameters)
{

   variableType** ppTempParameters;
   BOOL fWTA = FALSE;

   if ( ((UA*)pUser)->iUserAgentMode == WTA_USER_AGENT)
     fWTA = TRUE;

   ppTempParameters = (variableType**)*pppInParameters;
   if (ppTempParameters)
   {
	   while (*ppTempParameters)
	   {
          WML_SetVariable ((UA*)pUser, (*ppTempParameters)->name, (*ppTempParameters)->value, fWTA );
		  DEALLOC(& (*ppTempParameters)->value );
		  DEALLOC(& (*ppTempParameters)->name );
		  DEALLOC( ppTempParameters ); 
		  ppTempParameters++;
	   }
	   DEALLOC(pppInParameters);
   }
}

void SDL_CallFunction( int iTID, void** pRequestStruct ) 
{
	URLREQSTRUCT*	pUrlReq = NULL;
	BYTE*	pbQuery = NULL;
	BYTE*	pbHost = NULL;
	BYTE*	pbPath = NULL;

	if( *pRequestStruct != NULL )
	{
		pUrlReq = (URLREQSTRUCT*)(*pRequestStruct);
		if( pUrlReq != NULL && pUrlReq->pbURL != NULL )
		{
            if (pUrlReq->pbURL && b_GetQuery(pUrlReq->pbURL, & pbQuery) && b_GetPath(pUrlReq->pbURL, & pbPath) && b_GetHost(pUrlReq->pbURL, & pbHost) )
			  CLNTa_callFunction( (UINT8)iTID, (CHAR*)pbHost, (CHAR*)pbPath+1, (CHAR*)pbQuery );
			else
              SDL_OutputError(NULL, ERR_WAE_UA_URL_INVALID, ERRTYPE_INFORMATION );
			  	 
			DEALLOC(&pbPath);
			DEALLOC(&pbQuery);
			DEALLOC(&pbHost);
		}
		Delete_URLREQSTRUCT( &pUrlReq );
	}
}



/*---------------------------------------------------------------------------------
When using proxy/server authenication - copy username and password into one
string ("XXX\0XXX\0")
------------------------------------------------------------------------------------*/

int CopyCredentialParameters (void* pUserName, void* pPassWord, void** ppCredentials)
{
  int iLength = 0;
  int iNameLength = 0;
  int iPassLength = 0;
  BYTE *p;

  DEALLOC (ppCredentials);

  if (pUserName==NULL && pPassWord==NULL)
	  return 0;

  iNameLength = pUserName ? B_STRINGLENGTH (pUserName) : 0;
  iPassLength = pPassWord ? B_STRINGLENGTH (pPassWord) : 0;
  iLength = iNameLength + iPassLength + 2;

  *ppCredentials = NEWARRAY (BYTE, iLength);
  if (*ppCredentials == NULL) {
    return 0;
  }
  p = *ppCredentials;

  if (pUserName != NULL) {
    B_COPYSTRING ( (CHAR*)p, pUserName );
    #ifdef WAE_DEBUG
      PrintChar ("Username:");
      PrintChar (p);
    #endif
    p += iNameLength;
  }
  *p++ = '\0';  /*if username is null */

  if (pPassWord != NULL) {
    B_COPYSTRING ( (CHAR*)p, pPassWord );
    #ifdef WAE_DEBUG
      PrintChar ("Password:");
      PrintChar (p);
    #endif
    p += iPassLength;
  }
  *p = '\0';  /*if password is null */

  return iLength;
}

/*--------------------------------------------------------------------------------------------------------------------------------

                                                    TID MANAGEMENT

--------------------------------------------------------------------------------------------------------------------------------*/

#define EMPTY     0x0
#define INACTIVE  0x1
#define ACTIVE    0x2

#define MAX_TID   250

/* ---------------------------------------------
 * Returns a new TID from the TID pool. Return
 * value 0 indicates an error and is NOT a valid TID.
 * ----------------------------------------------*/
int SDL_CreateNewGlobalTID(void* pWAEInObj, int iViewID)
{
  int i;
  BYTE *tbl;
  WAEMAINOBJECT *pObj = (WAEMAINOBJECT *)pWAEInObj;

  iViewID = iViewID;  /* To get rid of a warning. */

  if (pObj == NULL)
    return 0;

  tbl = pObj->pTID_Table;
  
  if (tbl == NULL)
    return 0;

  /* To maximize the time between recycling a TID, we start searching
   * for an empty slot at the point where we left off last time. */
  for (i = (pObj->lastTID == MAX_TID) ? 1 : pObj->lastTID + 1;
       i != pObj->lastTID; (i == MAX_TID) ? i = 1 : i++) {
    int idx = (i >> 2);
    int n = (i & 0x3) << 1;

    if (((tbl[idx] >> n) & 0x3) == EMPTY) {
      tbl[idx] |= (ACTIVE << n);
      pObj->lastTID = (UINT8)i;
      return i;
    }
  }

  return 0;
}    


/* ---------------------------------------------
 * Return the next active TID strictly larger
 * than iPrevTID. If iPrevTID is 0, the smallest
 * active TID is returned.
 * If there is no active TID larger than iPrevTID,
 * this routine returns 0.
 * ----------------------------------------------*/
int SDL_GetNextActiveTID (void* pWAEInObj, int iPrevTID)
{
  int i;
  BYTE *tbl;

  if (pWAEInObj == NULL)
    return 0;

  tbl = ((WAEMAINOBJECT*)pWAEInObj)->pTID_Table;
  
  if (tbl == NULL)
    return 0;
  
  for (i = (iPrevTID == 0 ? 1 : iPrevTID + 1); i <= MAX_TID; i++) {
    int idx = (i >> 2);
    int n = (i & 0x3) << 1;

    if (((tbl[idx] >> n) & 0x3) == ACTIVE) {
      return i;
    }
  }

  return 0;
}

/* ---------------------------------------------
 * Deactivate a TID in the TID pool - i.e., the 
 * corresponding request is no longer active.
 * If the TID is not currently active, this is a no-op.
 * ----------------------------------------------*/
void SDL_DeactivateGlobalTID(void* pWAEInObj, int iTID)
{
  int idx = (iTID >> 2);
  int n = (iTID & 0x3) << 1;
  BYTE *tbl;

  if (pWAEInObj == NULL)
    return;

  tbl = ((WAEMAINOBJECT*)pWAEInObj)->pTID_Table;
  
  if (tbl == NULL)
    return;

  if (((tbl[idx] >> n) & 0x3) != ACTIVE) {
    return;
  }
  tbl[idx] &= ~(0x3 << n);
  tbl[idx] |= (INACTIVE << n);
} 

/* ---------------------------------------------
 * Check a TID in the TID pool - is the 
 * corresponding request active. Returns 0 (FALSE) if not active.
 * ----------------------------------------------*/
int SDL_CheckGlobalTIDStatus(void* pWAEInObj, int iTID)
{
  int idx = (iTID >> 2);
  int n = (iTID & 0x3) << 1;
  BYTE *tbl;

  if (pWAEInObj == NULL)
    return 0;

  tbl = ((WAEMAINOBJECT*)pWAEInObj)->pTID_Table;
  
  if (tbl == NULL)
    return 0;

  return ((tbl[idx] >> n) & 0x3) == ACTIVE;
} 

/* ---------------------------------------------
 * Releases a TID from the TID pool.
 * ----------------------------------------------*/
void SDL_ReleaseGlobalTID(void* pWAEInObj, int iTID)
{
  int idx = (iTID >> 2);
  int n = (iTID & 0x3) << 1;
  BYTE *tbl;

  if (pWAEInObj == NULL)
    return;

  tbl = ((WAEMAINOBJECT*)pWAEInObj)->pTID_Table;
  
  if (tbl == NULL)
    return;

  tbl[idx] &= ~(0x3 << n);
  tbl[idx] |= (EMPTY << n);
}

/* ---------------------------------------------
 * Deactivate ALL TIDs in the TID pool - i.e., the 
 * corresponding requests are no longer active.
 * ----------------------------------------------*/
void SDL_DeactivateAllGlobalTIDs(void* pWAEInObj)
{
  int i;
  BYTE *tbl;

  if (pWAEInObj == NULL)
    return;

  tbl = ((WAEMAINOBJECT*)pWAEInObj)->pTID_Table;
  
  if (tbl == NULL)
    return;

  for (i = 1; i <= MAX_TID; i++) {
    int idx = (i >> 2);
    int n = (i & 0x3) << 1;

    if (((tbl[idx] >> n) & 0x3) == ACTIVE) {
      tbl[idx] &= ~(0x3 << n);
      tbl[idx] |= (INACTIVE << n);
    }
  }
}                     

/* -----------------------------------------------------------------------------------------------
 *
 *                                            LISTHANDLING
 *
 * -----------------------------------------------------------------------------------------------
 */

/* --------------------------------------------------
 * Create a new list that will hold a maximum of
 * iMax elements. If iMax = 0, no upper limit is
 * enforced.
 * -------------------------------------------------- */
void* SDL_NewList (int iMax)
{
   LISTHEADER* pTheList;

   if ((pTheList = NEWSTRUCT (LISTHEADER)) == NULL)
     return NULL;

   pTheList->iMax = iMax;
   pTheList->iTotal = 0;
   pTheList->head.pNext = pTheList->head.pPrev = &(pTheList->head);

   return pTheList;
}


/* --------------------------------------------------
 * Remove all elements in the list, but NOT the data
 * that each element points to; it must be deallocated
 * separately. Total number of elements is set to 0.
 * The listheader is saved intact, and will represent
 * an empty list.
 * -------------------------------------------------- */
void SDL_ClearList (void* pList)
{
  LISTELEMENT* pHead;

  if (pList == NULL)
    return;

  pHead = &(((LISTHEADER *)pList)->head);
  while (pHead->pNext != pHead) {
    SDL_DeleteListElement (pList, pHead->pNext);
  }
}

/* --------------------------------------------------
 * Delete all listelements and then delete the list
 * header; this does NOT delete the data that each
 * element points to, it must be deallocated separately.
 * To be used when a COMPLETE removal of the list is wanted.
 * -------------------------------------------------- */
void SDL_DeleteList (void** ppList)
{
  /* Pass through all elements and deallocate them. */
  SDL_ClearList (*ppList);
  /* Remove the list header. */
  DEALLOC ((LISTHEADER**)ppList);
}

/* --------------------------------------------------
 * Add a new element to the list.
 * Puts pElement in the list and returns 1 (TRUE) if OK.
 * If iPrio != 0 then the list is a priority list, where
 * a iPrio controls the point of insertion; otherwise,
 * the element is inserted at the end of the list.
 * -------------------------------------------------- */
int
SDL_AddListElement (void* pList, int iId, int iPrio ,void* pElement)
{
  LISTHEADER* pTheList = (LISTHEADER*)pList;
  LISTELEMENT *pHead;
  LISTELEMENT *pBefore, *pNew;

  if ((pTheList == NULL) ||
      (((pTheList->iMax) != 0) && (pTheList->iTotal >= pTheList->iMax)))
    return 0;

  pHead = &(pTheList->head);
  if (iPrio != 0) {
    for (pBefore = pHead->pNext; pBefore != pHead; pBefore = pBefore->pNext) {
      if (pBefore->iPrio < iPrio)
        break;
    }
  }
  else
    pBefore = pHead;

  if ((pNew = NEWSTRUCT (LISTELEMENT)) == NULL)
    return 0;

  pTheList->iTotal++;
  pNew->iIdentifier = iId;
  pNew->pTheElement = pElement;
  pNew->iPrio = iPrio;

  pNew->pNext = pBefore;
  pNew->pPrev = pBefore->pPrev;
  pNew->pNext->pPrev = pNew;
  pNew->pPrev->pNext = pNew;

  return 1;
}


/* --------------------------------------------------
 * Add a new element last in the list.
 * Returns 1 (TRUE) if OK, 0 otherwise.
 * -------------------------------------------------- */
int
SDL_AddElementLast (void* pList, int iId, void* pElement)
{
  if (pList == NULL)
    return 0;

  return SDL_AddElementAfter (pList, iId,  pElement,
                              ((LISTHEADER *)pList)->head.pPrev);
}

/* --------------------------------------------------
 * Add a new element first in the list.
 * Returns 1 (TRUE) if OK, 0 otherwise.
 * -------------------------------------------------- */
int
SDL_AddElementFirst (void* pList, int iId, void* pElement)
{
  if (pList == NULL)
    return 0;

  return SDL_AddElementAfter (pList, iId,  pElement,
                              &(((LISTHEADER *)pList)->head));
}

/*---------------------------------------------------
 * Add a new element after the one specified by pAfter.
 * If pAfter is NULL then add the new element first in the list.
 * Returns 1 (TRUE) if OK, 0 otherwise.
 * -------------------------------------------------- */
int
SDL_AddElementAfter (void* pList, int iId, void* pElement, void* pAfter)
{
  LISTELEMENT* pNew = NULL;
  LISTHEADER* pTheList = (LISTHEADER *)pList;

  if ((pTheList == NULL) ||
      (((pTheList->iMax) != 0) && (pTheList->iTotal >= pTheList->iMax)))
    return 0;

  if ((pNew = NEWSTRUCT (LISTELEMENT)) == NULL)
    return 0;

  pTheList->iTotal++;
  pNew->iIdentifier = iId;
  pNew->pTheElement = pElement;
  pNew->iPrio = 0;

  if (pAfter == NULL)
    pAfter = &(pTheList->head);
  
  pNew->pNext = ((LISTELEMENT*)pAfter)->pNext;
  pNew->pPrev = (LISTELEMENT*)pAfter;
  ((LISTELEMENT*)pAfter)->pNext = pNew;
  pNew->pNext->pPrev = pNew;

  return 1;
}

/* --------------------------------------------------
 * Get the element with id iID. Start searching from the
 * pSearchFrom listelement; if pSearchFrom is NULL then 
 * the search is performed from the beginning.  
 * If a matching element is found this is returned,
 * otherwise NULL.
 * -------------------------------------------------- */
void *
SDL_SearchListElement (void* pList, int iId, void* pSearchFrom)
{
  LISTELEMENT *pReturnElement = NULL;
  LISTELEMENT *pHead;

  if (pList == NULL)
    return NULL;

  pHead = &(((LISTHEADER *)pList)->head);

  if (pSearchFrom == NULL)
    pSearchFrom = pHead->pNext;

  for (pReturnElement = (LISTELEMENT*)pSearchFrom; pReturnElement != pHead;
       pReturnElement = pReturnElement->pNext) {
    if (pReturnElement->iIdentifier == iId)
      break;
  }

  return (pReturnElement == pHead) ? NULL : pReturnElement;
}


/*-------------------------------------------------
 * Get the next listelement. If pTheReqElm is NULL,
 * then the first element is returned.
 * -------------------------------------------------- */
void *
SDL_GetNextListElement (void* pList, void* pTheReqElm)
{
  LISTELEMENT *pHead;
  LISTELEMENT *pElement = (LISTELEMENT *)pTheReqElm;

  if (pList == NULL)
    return NULL;

  pHead = &(((LISTHEADER *)pList)->head);

  if (pElement == NULL)
    pElement = pHead;

  return (pElement->pNext == pHead) ? NULL : pElement->pNext;
}

/* -------------------------------------------------
 * Gets the content of a listelement, i.e., the generic
 * data pointed to.
 * --------------------------------------------------*/
void *
SDL_GetListElementContent (void* pTheReqElm)
{
  if (pTheReqElm == NULL)
    return NULL;

  return ((LISTELEMENT*)pTheReqElm)->pTheElement;
}

/* --------------------------------------------------
 * Change the content of a LISTElement, i.e., the generic
 * data pointed to.
 * -------------------------------------------------- */
void
SDL_ChangeListElementContent (void* pTheReqElm, void* pNewContentElement)
{
  if (pTheReqElm != NULL)
    ((LISTELEMENT*)pTheReqElm)->pTheElement = pNewContentElement;
}

/* --------------------------------------------------
 * Get the id of a listelement.
 * -------------------------------------------------- */
int
SDL_GetListElementID (void* pTheReqElm)
{
  if (pTheReqElm == NULL)
    return 0;

  return ((LISTELEMENT*)pTheReqElm)->iIdentifier;
}

/* --------------------------------------------------
 * Change the id of a LISTElement.
 * -------------------------------------------------- */
void
SDL_ChangeListElementID (void* pTheReqElm, int iId )
{
  if (pTheReqElm != NULL)
    ((LISTELEMENT*)pTheReqElm)->iIdentifier = iId;
}

/* -------------------------------------------------
 * Delete the listelement pRElement. Does NOT delete
 * the generic data pointed to by the element.
 * --------------------------------------------------*/
void
SDL_DeleteListElement (void* pList, void* pRElement)
{
  LISTHEADER *pTheList = (LISTHEADER *)pList;
  LISTELEMENT* pRemoveElement = (LISTELEMENT *)pRElement;

  if ((pTheList == NULL) || (pRElement == NULL) || (pTheList->iTotal <= 0))
    return;

  pTheList->iTotal--;
  pRemoveElement->pPrev->pNext = pRemoveElement->pNext;
  pRemoveElement->pNext->pPrev = pRemoveElement->pPrev;
  DEALLOC (&pRemoveElement);
}


/* --------------------------------------------------
 * Get the number of elements in a list.
 * -------------------------------------------------- */
int
SDL_GetCardinal (void* pList)
{
  if (pList == NULL)
    return 0;

  return ((LISTHEADER*)pList)->iTotal;
}

/*---------------------------------------------------------------------------------------------------------
                
				                         Stackfunctions

-----------------------------------------------------------------------------------------------------------*/

void* SDL_NewLIFOStack(void)
{
  return SDL_NewList(0);
}

void SDL_ClearLIFOStack( void** ppStack )
{
   SDL_DeleteList(ppStack);
}

void* SDL_POPLIFOStack(void* pStack)
{
   void* pListElm=NULL;
   void* pReturn;
   /* get the first element */
   pListElm = SDL_GetNextListElement(pStack, NULL);
   pReturn = SDL_GetListElementContent(pListElm);
   SDL_DeleteListElement(pStack, pListElm);
   return pReturn;
}

int SDL_PUSHLIFOStack( void* pStack, void* pElement )
{
   return SDL_AddElementFirst(pStack, 0, pElement);
}

void* SDL_CheckLIFOStack( void *pStack )
{
   void* pListElm=NULL;
   pListElm = SDL_GetNextListElement(pStack, NULL);
   return SDL_GetListElementContent(pListElm);
}


/*------------------------------------------------------------
                 General
--------------------------------------------------------------*/


/*---------------------------------------------
Enables SDL to dynamically free structures in C
e.g. strings
Input: Datapointer
Output: -
----------------------------------------------*/
void SDL_Free(void** pTheData)
{
    DEALLOC(pTheData);
}

/*---------------------------------------------
Enables SDL to output errormessages to the application
Input: errNo , defined in the porting guide.
Output:
----------------------------------------------*/

void SDL_OutputError(void* pUser, int iErrorcode, int iErrType)
{
   if (pUser==NULL)
     CLNTa_error(0, (INT16)iErrorcode, (UINT8)iErrType);
   else
     CLNTa_error(((UA*)pUser)->iViewID, (INT16)iErrorcode, (UINT8)iErrType);
}

void SDL_OutputStatus(void* pUser, int iStatus, void* pURL )
{
   MMIa_status( ((UA*)pUser)->iViewID, (UINT8)iStatus, (CHAR*)pURL );
}

void SDL_LOG(void* pUser, int logno, void* pString)
{
    /*  To get rid of warnings for unused variables */
    pUser = pUser;
    logno = logno;
    pString = pString;

 #ifdef LOG_INTERNAL 
   if (pUser==NULL)
     CLNTa_log( 0, (INT16)logno,  pString );
   else
     CLNTa_log( ((UA*)pUser)->iViewID, (INT16)logno,  pString );
 #endif
}


void* GenCopyString(INT8 iSize, const void* pStr)
{
   void* pRes=NULL;
   if (pStr !=NULL)
   {
     if (iSize == 1)
     {
       pRes = NEWARRAY(BYTE,B_STRINGLENGTH(pStr)+1);
	   if (pRes)
         B_COPYSTRING(pRes,pStr);
     }
     else
     {
       pRes = NEWARRAY(WCHAR,STRINGLENGTH(pStr)+1);
	   if (pRes)
         COPYSTRING(pRes,pStr);
     }
   }
   return pRes;
}

#endif
