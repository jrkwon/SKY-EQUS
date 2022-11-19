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
	FILE: History.c
	
	General History handler for WAE

	Rev history:
		991124	JPR		Created, functions moved from WMLIF.c
		991124	FAR		A generic history functionality model added

=========================================================================*/

#include "history.h"
#include "wmlif.h"

/*---------------------------------- generic History functions, ------------------------------------------*/

void Delete_HistoryItem(void ** ppHistoryList)
{
  HISTORYITEM *tmp;

	if (!(ppHistoryList && *ppHistoryList))
		return;

/* gbu 000211   cast added
  tmp = *ppHistoryList;
*/
  tmp = (HISTORYITEM *)(*ppHistoryList);
  if (!tmp->inList) {
    DEALLOC (&(tmp->pwchItemTitle));
    DEALLOC (&(tmp->pbURLEntry));
    DEALLOC (&(tmp->pbPostData));

    DEALLOC (ppHistoryList);
  }
  *ppHistoryList = NULL;
}

HISTORYITEM* New_HistoryItem(void)
{
   HISTORYITEM*  pHistoryItem;
   pHistoryItem = NEWSTRUCT(HISTORYITEM);

   if (pHistoryItem)
   {
     pHistoryItem->inList = 0;
      pHistoryItem->pwchItemTitle = NULL;
      pHistoryItem->pbURLEntry = NULL;
      pHistoryItem->pbPostData = NULL;
	  pHistoryItem->iPostDataLength = 0;
	  pHistoryItem->iMethod = M_Get;
      pHistoryItem->iContentType = 0;
	  pHistoryItem->iContentCharSet =0;
      pHistoryItem->pNextEntry = NULL;
   }
   return pHistoryItem;
}

/*
HISTORYITEM* DefineNewHistoryItem(BYTE **pbURLEntry, WCHAR **pchItemTitle, BYTE* pbPostData, UINT16 iPostDataLength, UINT8 iMethod, UINT8 iContentType)
{
   HISTORYITEM*  pHistoryItem;
   pHistoryItem = New_HistoryItem();

   if (pHistoryItem)
   {
      if (pchItemTitle)
	  {
         pHistoryItem->pchItemTitle = *pchItemTitle;
	     *pchItemTitle = NULL;
      }
      if (pbURLEntry)
	  {
		 pHistoryItem->pbURLEntry = *pbURLEntry;
	     *pbURLEntry = NULL;
	  }

      pHistoryItem->pbPostData = pbPostData;

	  pHistoryItem->iPostDataLength = iPostDataLength;
	  pHistoryItem->iMethod = iMethod;
      pHistoryItem->iContentType = iContentType;
      pHistoryItem->pNextEntry = NULL;
   }
   return pHistoryItem;
}
*/

HISTORYITEM* CreateAndInitNewHistoryItem(BYTE *pbURLEntry, WCHAR *pwchItemTitle, BYTE* pbPostData, UINT16 iPostDataLength, UINT8 iMethod, UINT8 iContentType, INT16 iContentCharSet)
{
   HISTORYITEM*  pHistoryItem;
   pHistoryItem = NEWSTRUCT(HISTORYITEM);

   if (pHistoryItem)
   {
     pHistoryItem->inList = 0;
      pHistoryItem->pwchItemTitle = pwchItemTitle;
      pHistoryItem->pbURLEntry = pbURLEntry;
      pHistoryItem->pbPostData = pbPostData;
      pHistoryItem->iPostDataLength = iPostDataLength;
	  pHistoryItem->iMethod = iMethod;
      pHistoryItem->iContentType = iContentType;
	  pHistoryItem->iContentCharSet = iContentCharSet;
      pHistoryItem->pNextEntry = NULL;
   }
   return pHistoryItem;
}


/*	EmptyHistoryList  (clears the list from ppHistoryList) */
void EmptyHistoryList (HISTORYITEM ** ppHistoryList) 
{
	HISTORYITEM* pTempHistoryItem;
	HISTORYITEM* pNextHistoryItem=NULL;

	if (ppHistoryList)
	   pNextHistoryItem = *ppHistoryList;
	while (pNextHistoryItem!=NULL) 
	{
		pTempHistoryItem = pNextHistoryItem;
		pNextHistoryItem = pTempHistoryItem->pNextEntry;
    pTempHistoryItem->inList = 0;
    Delete_HistoryItem((void**)(&pTempHistoryItem));
	}
	*ppHistoryList = NULL;

}



HISTORYITEM* HistoryRead (HISTORYITEM* pHistoryList, int iPosition)
{
	iPosition--;
	while (pHistoryList!=NULL && iPosition>0) 
	{
		pHistoryList=pHistoryList->pNextEntry;
		iPosition--;
	}
    return pHistoryList;
}

/* pops the history stack (deletes the element) and returns the new topmost element */
void HistoryListPop (HISTORYITEM** ppHistoryList) 
{
	HISTORYITEM* pNextHistoryItem = NULL;
	if (*ppHistoryList!=NULL) 
	{
		pNextHistoryItem=(*ppHistoryList)->pNextEntry;
		/* Remove first entry. */
    (*ppHistoryList)->inList = 0;
		Delete_HistoryItem((void**)ppHistoryList);

	}
	*ppHistoryList=pNextHistoryItem;
}

HISTORYITEM* HistoryListPush(HISTORYITEM* pHistoryList, HISTORYITEM* pNewHistoryItem, UINT16 iMax )
{
/* gbu 000208
	HISTORYITEM* pNextHistoryItem = pHistoryList;
*/
/* aspn 001101
	UINT16 iTotal =0;
*/
	if (pNewHistoryItem) {
    pNewHistoryItem->inList = 1;
	  pNewHistoryItem->pNextEntry = pHistoryList;
/*    iTotal= DeleteLastHistoryItems(pNewHistoryItem, iMax); aspn 001101*/
    DeleteLastHistoryItems(pNewHistoryItem, iMax);   /* remove any extra items */
  }

  return pNewHistoryItem;
}

/* 	HistoryDeleteLeastUsed returns the number of elements available in the history */
UINT16 DeleteLastHistoryItems(HISTORYITEM* pHistoryList, UINT16 iMax)
{
/* gbu 000208
	HISTORYITEM* pEntry=NULL;
*/
/* gbu 000208
	HISTORYITEM* pTempEntry=NULL;
*/
    UINT16 iTotal = 0;
    UINT16 iMaximum = iMax;

	if (pHistoryList==NULL)
		return 0;

    /* find the first element to remove */
	while (pHistoryList && iMaximum>1)
	{
	   iTotal++;
	   iMaximum--;
	   pHistoryList=pHistoryList->pNextEntry;
	}

	if (iMaximum==1)
       iTotal = iMax;
	else
	   iTotal++;

    if (pHistoryList)
	    EmptyHistoryList (& pHistoryList->pNextEntry );

	return iTotal;
}

UINT16 GetHistoryTotal(HISTORYITEM* pHistoryList)
{
	UINT16 iTotal=0;
   	while (pHistoryList)
	{
	   iTotal++;
	   pHistoryList=pHistoryList->pNextEntry;
	}
	return iTotal;
}

/*---------------------------------- WML specific History functions, ------------------------------------------*/

/*========================================================================
	EmptyHistoryList  (clears the list from ppHistoryList)
	WML_EmptyHistoryList 
==========================================================================*/

void WML_EmptyHistoryList (pUA pUser) 
{
	if (pUser!=NULL) 
	{
/* gbu 00211    no cast needed
		EmptyHistoryList( & ((HISTORYITEM*)(pUser->pHistoryList)) );
*/
        EmptyHistoryList( &pUser->pHistoryList);
		/* Set counter to 0 */
	/*	pUser->iElmInHistory=0; */
	}
}

/*========================================================================
	WML_EmptyHistoryNewContext
==========================================================================*/
void WML_EmptyHistoryNewContext (pUA pUser)
{

	HISTORYITEM* pTop;

	if (pUser!=NULL) 
	{
		/* Set temp to the first entry */
		pTop=pUser->pHistoryList;

		if (pTop!=NULL)
		{
			/* Step one in the history list */
			pUser->pHistoryList=((HISTORYITEM*)pUser->pHistoryList)->pNextEntry;

			/* Remove rest of history list */
/* gbu 0002011 no cast needed
		    EmptyHistoryList( & ((HISTORYITEM*)(pUser->pHistoryList)) );
*/
            EmptyHistoryList( &pUser->pHistoryList );

			pTop->pNextEntry=NULL;

        }

		/* Store temp in history list */
		pUser->pHistoryList=pTop;
	}
}


/*========================================================================
	HistoryRead (and WML_HistoryRead - read a specific URL entry..)
==========================================================================*/

BYTE *WML_HistoryRead (pUA pUser, int iPosition)
{
	HISTORYITEM* pTempEntry=NULL;
	BYTE *pbResult=NULL;

	if (pUser!=NULL) 
	{

		pTempEntry=HistoryRead (pUser->pHistoryList, iPosition);

		if (pTempEntry!=NULL)
		{
			if (pTempEntry->pbURLEntry !=NULL)
			{
				pbResult=NEWARRAY(BYTE,B_STRINGLENGTH((const char *)pTempEntry->pbURLEntry)+1);

				if (pbResult!=NULL)
				{
					B_COPYSTRING((CHAR*)pbResult,(CHAR*)pTempEntry->pbURLEntry);	  
				}
			}
		}
	}
	return pbResult;
}


/*========================================================================
	HistoryListPop 
=========================================================================*/
void WML_HistoryListPop ( UA* pUser)
{
   HistoryListPop(& (pUser->pHistoryList) ); 

}

/*========================================================================
	WML_HistoryListPush
==========================================================================*/


BOOL WML_HistoryListPush (pUA pUser, BYTE *pbURLEntry, WCHAR *pwchItemTitle , HISTORYITEM* pHistoryItem)
{
	BOOL fError=FALSE;
/* gbu 000208
	BYTE *pbNewEntry=NULL;
*/
/* gbu 000208
	WCHAR *pwchNewTitle=NULL;
*/
/* gbu 000208
	UINT16 iEntryLen=0;
*/

	if (pUser!=NULL)
	{
		if (pbURLEntry!=NULL)
		{
            if (!pHistoryItem)
                pHistoryItem = New_HistoryItem();

			if (pbURLEntry)  /* replace any URI */
			{
                DEALLOC(&pHistoryItem->pbURLEntry);
				pHistoryItem->pbURLEntry = NEWARRAY(BYTE,B_STRINGLENGTH((const char *)pbURLEntry)+1);
				if (pHistoryItem->pbURLEntry)
					B_COPYSTRING((CHAR*)pHistoryItem->pbURLEntry,(CHAR*)pbURLEntry);
			}

			if (pwchItemTitle)  /* replace any Title */
			{
                DEALLOC(&pHistoryItem->pwchItemTitle);
				pHistoryItem->pwchItemTitle = NEWARRAY(WCHAR,STRINGLENGTH(pwchItemTitle)+1);
				if (pHistoryItem->pwchItemTitle)
					COPYSTRING(pHistoryItem->pwchItemTitle,pwchItemTitle);
			}

			if (pHistoryItem->pwchItemTitle == NULL)
			{
				/* Add empty string instead of NULL-pointer.
				   Otherwise the function WML_GetHistoryListTitles
				   will not execute properly. */
				pHistoryItem->pwchItemTitle=NEWARRAY(WCHAR,1);
				pHistoryItem->pwchItemTitle[0]=0;

			}

            pUser->pHistoryList = HistoryListPush(pUser->pHistoryList, pHistoryItem, pUser->iMaxElmInHistory );


		}
	}
	return fError;
}


/*========================================================================
	WML_SetMaxInHistoryList
==========================================================================*/
void WML_SetMaxInHistoryList (pUA pUser, UINT16 iMaxElements)
{

	if (iMaxElements==0)
		iMaxElements = 10;

	DeleteLastHistoryItems(pUser->pHistoryList, iMaxElements);
	pUser->iMaxElmInHistory=iMaxElements;


}


/*========================================================================
	WML_GetHistoryListTitles
==========================================================================*/
WCHAR **WML_GetHistoryListTitles (pUA pUser)
{
	WCHAR **ppchTitles=NULL;
	WCHAR **ppchTempTitles=NULL;
	UINT16 iCount=0;
	HISTORYITEM* pTempEntry=NULL;

	iCount = GetHistoryTotal(pUser->pHistoryList);

	if (pUser!=NULL && iCount)
	{
		pTempEntry=pUser->pHistoryList;

  	    ppchTitles=NEWARRAY(WCHAR*,iCount+1);
 		if (ppchTitles!=NULL)
		{
			ppchTempTitles=ppchTitles;
			while ((pTempEntry!=NULL)&&(iCount--))
			{
				*ppchTempTitles++=pTempEntry->pwchItemTitle;
				pTempEntry=pTempEntry->pNextEntry;
			}
			*ppchTempTitles=NULL;
		}
	}
	return ppchTitles;
}


void SDL_GetPreviousCardURL(void* pUser, void** ppURL, void** ppHistoryItem )
{
  *ppURL = NULL;
  if (GetHistoryTotal(((UA*)pUser)->pHistoryList) > 1)
  {
     *ppURL = WML_HistoryRead((UA*)pUser,2);
	 *ppHistoryItem = HistoryRead ( ((UA*)pUser)->pHistoryList, 2);
  }

}

void SDL_PutInHistory(void* pUser, void* pInUrl, void* pCurrentCard, void* pHistoryItem)
{
  WCHAR* pwchTitle;
  if ((pUser!=NULL) && (pCurrentCard!=NULL))
  {
    if ( pInUrl !=NULL)
    {
       pwchTitle = WML_GetCardTitle ( (UA*)pUser, (ELEMENTTYPE*)pCurrentCard);

       WML_HistoryListPush ( (UA*)pUser, (BYTE*)pInUrl, pwchTitle, (HISTORYITEM*)pHistoryItem );
       
       #ifdef WAE_DEBUG
         PrintChar("Puthistory:");
         WMLUA_PrintWS(pwchTitle);
         PrintChar(pInUrl);
       #endif

         DEALLOC(&pwchTitle);
    }
  }
}
