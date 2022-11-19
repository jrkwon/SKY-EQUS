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
	FILE: WMLEvent.h

	Event handler for WMLParse.

	Rev history:
		980812	JPR		Created
		991223	JPR		Corrections
		990311	JPR		Function name changes. AddToEventTable and
						WML_DeleteEventTab added.
		990317	JPR		Corrections
   ================================================================= */

#ifndef _EVENTHANDLERH
#define _EVENTHANDLERH

#include "WMLToken.h"
#include "WMLDef.h"


/* ----------- EVENT TABLE FUNCTIONS ----------- 
	   Functions for handling the Events.
   --------------------------------------------- */
/* ----------------------------------------------
   If the WCHAR stream is at least four characters 
   long and begins with '(V|v)(N|n)(D|d).' they are
   changed to 'vnd.'.
   Input: Pointer to WCHAR *
   Output: -
   ----------------------------------------------*/
void WML_ChangeVND (WCHAR **ppchEventType);

/* ----------------------------------------------
   Creates an event with the specified data.
   Input: Event Type (INT8) and ELEMENT 
		  to be executed when the event occurs.
   Output: EVENT if OK, NULL otherwise.
   ----------------------------------------------*/
pEVENT WML_CreateEvent (INT8 iEvent, pELEMENTTYPE pAction);

/* ----------------------------------------------
   Removes an event from the specified event table.
   Note! The action for the event is NOT removed.
		 This element is removed when the WML-
		 structure is removed. 
   Input: EVENT and Event table
   Output: -
   ----------------------------------------------*/
void WML_DeleteEvent (pEVENT pEvent, pEVENT *ppEventTab);

/* ----------------------------------------------
   Inserts an event into the specified eventtable.
   Input: EVENT and Event table. 			
   Output: -.
   ----------------------------------------------*/
void WML_AddEvent (pEVENT pEvent, pEVENT *ppEventTab);

/* ----------------------------------------------
   Returns the Value (ie. the action to be executed)
   for the specified event.
   Input: Event Type (INT8) and Eventtable					
   Output: Pointer to ELEMENT
   ----------------------------------------------*/
pELEMENTTYPE WML_GetEventAction (INT8 iEvent, pEVENT pEventTab);

/* ------------------------------------------
   Creates and adds an event to the event table
   in the specified element (CARD or TEMPLATE). 
   Input: UA, Element, Event and Action
   Output: TRUE if OK, FALSE otherwise
   ------------------------------------------*/
BOOL WML_AddToEventTable (pELEMENTTYPE pElementObj, INT8 iEvent, pELEMENTTYPE pAction);

/* ----------------------------------------------
   Deletes the specified eventtable.
   Input: Eventtable					
   Output: -
   ----------------------------------------------*/

void WML_DeleteEventTab (pEVENT pEventTab);
/* --------------------------------------------- */

#endif

