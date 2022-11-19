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
/* ==================================================================
	FILE: WMLEvent.c

	Event handler for WMLParse.

	Rev history:
		980812	JPR		Created
		991223	JPR		Corrections
		990311	JPR		Function name changes. AddToEventTable and
						WML_DeleteEventTab added.
		990317	JPR		Minor corrections.
   ================================================================= */

#include "WMLEvent.h"
#include "WMLDef.h"
#include "CmmnRsrc.h"

/* Change any '(v|V)(n|N)(d|D).' to 'vnd.'   */
void WML_ChangeVND (WCHAR **ppchEventType)
{
	WCHAR *pchTemp=NULL;
		
	if (*ppchEventType!=NULL)
	{
		/* Check length */
		if (STRINGLENGTH(*ppchEventType)>=4)
		{
			pchTemp=*ppchEventType;

			if ((*pchTemp==WC('V')||*pchTemp==WC('v'))&&
				(*(pchTemp+1)==WC('N')||*(pchTemp+1)==WC('n'))&&
				(*(pchTemp+2)==WC('D')||*(pchTemp+2)==WC('d'))&&
				(*(pchTemp+3)==WC('.')))
			{
				*pchTemp=WC('v');
				*(pchTemp+1)=WC('n');
				*(pchTemp+2)=WC('d');
			}
		}
	}
}

pEVENT WML_CreateEvent (INT8 iEvent, pELEMENTTYPE pAction) 
{
	pEVENT pTempEvent;

	pTempEvent=NEWSTRUCT(EVENT);

	if (pTempEvent!=NULL)
	{
		pTempEvent->iEvent=iEvent;
		pTempEvent->pAction=pAction;
		pTempEvent->pNextEvent=NULL;
	}
	return pTempEvent;
}

void WML_DeleteEvent (pEVENT pEvent, pEVENT *ppEventTab) 
{
	pEVENT pTempEvent=*ppEventTab;
	pEVENT pTempPar=NULL;

	while (pTempEvent!=NULL) 
	{
		/* Compare the pointers */
		if (pTempEvent==pEvent) 
		{
			if (pTempPar!=NULL)
			{
				pTempPar->pNextEvent=pTempEvent->pNextEvent;
			}
			else
			{
				*ppEventTab=pTempEvent->pNextEvent;
			}

			/* The action for the event is NOT removed. This 
			   element is removed when the WML-structure is 
			   removed. */
			pTempEvent->pAction=NULL;

			/* Delete event */
			DEALLOC(&pTempEvent);
			return;
		}
		else 
		{
			pTempEvent=pTempEvent->pNextEvent;
		}
		pTempPar=pTempEvent;
	}
}

/* Puts the event first in the actual event list */
void WML_AddEvent (pEVENT pEvent, pEVENT *ppEventTab) 
{
	if (pEvent!=NULL)
	{
		if (*ppEventTab!=NULL) 
		{
			pEvent->pNextEvent=*ppEventTab;
			*ppEventTab=pEvent;
		} 
		else
		{
			*ppEventTab=pEvent;
		}
	}
}

pELEMENTTYPE WML_GetEventAction (INT8 iEvent, pEVENT pEventTab) 
{
	pEVENT pTempEvent=pEventTab;
	
	/* Check if iEvent is not TYPE_STRING 
	   (i.e. well defined TYPE) */
	if (pEventTab!=NULL) 
	{
		while (pTempEvent!=NULL) 
		{
			if (pTempEvent->iEvent==iEvent)
			{
				return pTempEvent->pAction;
			}
			pTempEvent=pTempEvent->pNextEvent;
		}
	}
	return NULL;
}


BOOL WML_AddToEventTable (pELEMENTTYPE pElementObj, INT8 iEvent, pELEMENTTYPE pAction) 
{
	pEVENT pTempEvent;

	/* Check if the event container is defined. */
	if (pElementObj!=NULL) 
	{	
		/* Only events in CARD or TEMPLATE */
		if ((pElementObj->iType==Type_CARD)||(pElementObj->iType==Type_TEMPLATE)) 
		{	
			pTempEvent=WML_CreateEvent(iEvent,pAction);
			if (pTempEvent!=NULL) 
			{
				if (pElementObj->iType==Type_CARD)
				{
					WML_AddEvent(pTempEvent,&(((CARDELEMENT *)
									(pElementObj->pElm))->pEventTab));
				}
				else
				{
					WML_AddEvent(pTempEvent,&(((TEMPLATEELEMENT *)
									(pElementObj->pElm))->pEventTab));
				}
				return TRUE;
			} 
		} 
	}
	return FALSE;
}


/* Deletes the specified eventtable */
void WML_DeleteEventTab (pEVENT pEventTab) 
{
	pEVENT pTempEvent;
	pEVENT pTempEventPar=pEventTab;
	
	while (pTempEventPar!=NULL) 
	{
		pTempEvent=pTempEventPar->pNextEvent;
		WML_DeleteEvent(pTempEventPar,&pEventTab);
		pTempEventPar=pTempEvent;
	}
}

