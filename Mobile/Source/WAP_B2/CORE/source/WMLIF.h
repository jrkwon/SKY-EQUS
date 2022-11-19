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
	FILE: WMLIF.h
  
	Source file for the interface between WAE and the WML-structure.
	Includes function to initiate the parse of a WML-deck and using
	the structure created during the parse.

	Rev history:
		980728	JPR		Created
		990208	JPR		Corrections						
		990315	JPR		Major changes. Handles the WML-specific parts 
						of the startup and closing down of the parser.
		990316	JPR		Variable handling moved here from WMLElm.h
		990406	JPR		Corrections
		990423	JPR		Corrections in History functions
		990514	JPR		WML/WBXML Version handling updated.
		990528	JPR		Function WML_IsPEmpty added
		990531	JPR		Navigation corrections (setvar element)
		990610	JPR		Trim text functions updated.
		990826	JPR		Function WML_NewContext added.
   ================================================================== */

#ifndef _INTERFACEH
#define _INTERFACEH

#include "WMLDef.h"

typedef struct tNUMBERSTRUCT
{
	UINT16 iIndex;
	struct tNUMBERSTRUCT *pNext;
} NUMBERSTRUCT, *pNUMBERSTRUCT;


/* ----------- WML PARSER FUNCTIONS ------------
	Functions used to parse a WML-deck. The 
	functions uses the general WBXML-decoder
	(WBXMLDec) and for the WML-specific parts it
	uses WMLParse.

	The functions must be used in the following 
	manner:

	1. An user agent is created with the 
	   WML_CreateUserAgent function. The user 
	   agent contains information of the wml-
	   context and is used until it is terminated.

	2. For each wml-deck to be parsed, the function
	   WML_StartParser is called initially. If it
	   returns TRUE step 3 should be executed. 
	   Otherwise, step 4 should be executed.

	3. The function WML_ParseNextElement is called
	   repeatedly until NULL is returned which
	   indicated that the whole deck has been parsed.

	4. The function WML_EndParser is called if step (3)
	   is completed. The function returns TRUE if the 
	   parse was successful, FALSE otherwise. If step
	   (3) is interrupted earlier, the function 
	   WML_AbortParser must be caled instead.
	
	5. The functions WML_EmptyDECK, WML_EmptyVarTable,
	   DeleteHostList may be called to alter the
	   context of the user agent.

	6. Repeat step (2-4) for each new deck to be
	   parsed.

    7. Delete the user agent with the function
	   WML_DeleteUserAgent. The function also 
	   removes the variable table, the current
	   deck, the history list, the decode struct,
	   the HostList, and the URL Base.

   --------------------------------------------- */ 
/* ----------------------------------------------
	Creates an User Agent and sets the attributes
	to their default values.
	Input: -
	Output: pointer to UA, NULL if error
   ---------------------------------------------- */
pUA WML_CreateUserAgent (void);

/* ----------------------------------------------
	Deletes an User Agent and removes the attributes
	pbURLBase, pVisitedHosts, the variable table,
	the decode struct and the current deck.
	Input: ** to User Agent
	Output: -
   ---------------------------------------------- */
void WML_DeleteUserAgent (pUA *ppUser);

/* -----------------------------------------------
    Starts up the parser. Parses the prolog and 
	builds the string table. During this phase the
	version number (WBXML) is also verified. 
	Input: User Agent, pointer to bytestream, the 
		   number of bytes in the bytestream, the
		   content type and content level.
	Output: TRUE if OK, FALSE otherwise.
  ----------------------------------------------- */
BOOL WML_StartParser (pUA pUser, BYTE *pbStream, UINT32 iNbrOfBytes,
					  BYTE bContentType, UINT8 iContentLevel); 

/* -----------------------------------------------
    Finishes the parse of a deck. Removes the
	temporary variables. Stores all not overridden
	TEMPLATE DO	elements in the cards. If the parse
	was successful TRUE is returned, otherwise
	FALSE is returned. If the PARALLELL_PARSE_ENABLED
	is set, the result FALSE resumes the old deck.
	Input: User Agent
	Output: TRUE/FALSE
   ----------------------------------------------- */
BOOL WML_EndParser (pUA pUser);

/* -----------------------------------------------
    Aborts the parse of a deck. Removes the
	temporary variables. Stores all not overridden
	TEMPLATE DO	elements in the cards. If the 
	PARALLELL_PARSE_ENABLED is set the old deck is
	resumed.
	Input: User Agent
	Output: TRUE/FALSE
   ----------------------------------------------- */
BOOL WML_AbortParser (pUA pUser);

/* -----------------------------------------------
    Parses the wml content elements in the deck, 
	one by one. CARD, HEAD and TEMPLATE are at this
	level. The elements are stored in the User Agent, 
	but a pointer is also returned. 
	Input: User Agent
	Output: Element of type CARD, HEAD or TEMPLATE.
			Returns NULL if no more elements.
  ----------------------------------------------- */
pELEMENTTYPE WML_ParseNextElement (pUA pUser);

/* ----------------------------------------------
	Deletes the WML deck in the specified User Agent
	Input: Pointer to User Agent
	Output: -
   ---------------------------------------------- */
void WML_EmptyDECK (pUA pUser);

/* ============================================= */
/* ============================================= */





/* ---------- HISTORY LIST FUNCTIONS -----------
	Functions used for the history list.	
	Each User Agent contians a history list over
	visited URL:s. Five functions are supplied:
	Empty, push, pop, deleteLeastUsed and SetMax.
   --------------------------------------------- */ 
/* ----------------------------------------------
	Clears the History list in the specified 
	User Agent.
	Input: Actual User Agent
	Output: -
   ---------------------------------------------- */
void WML_EmptyHistoryList (pUA pUser);

/* ----------------------------------------------
	Removes all entries, but the top entry, in 
	the History list in the specified user Agent.
	Input: Actual User Agent
	Output: -
   ---------------------------------------------- */
void WML_EmptyHistoryNewContext (pUA pUser);

/* ----------------------------------------------
	Returns the a COPY of the URI at the specified
	position.

	Input: Actual User Agent and position
	Output: BYTE *, NULL not found.
   ---------------------------------------------- */
BYTE *WML_HistoryRead (pUA pUser, int iPosition);

/* ----------------------------------------------
	Returns the top entry URI in the history list 
	in the specified User Agent, and removes it 
	from the list. Everything is deallocated 
	besides the returned string.
	Input: Actual User Agent
	Output: Top entry (BYTE *), NULL if the list 
			is empty.
   ---------------------------------------------- */
BYTE *WML_HistoryListPop (pUA pUser);

/* ----------------------------------------------
	Adds a entry on the history list in the specified
	User Agent. Stores COPIES of the strings. If the
	number of elements exceeds the maximum allowed
	number, the least used element is removed.
    Input: Actual User Agent, entry (BYTE *) and
		   the title of the entry (WCHAR *)
	Output: TRUE if OK, FALSE otherwise.
   ---------------------------------------------- */
BOOL WML_HistoryListPush (pUA pUser, BYTE *pbEntry, WCHAR *pchTitle);

/* ----------------------------------------------
	Deletes the least used item in the history
	list.
	Input: Actual User Agent
	Output: -
   ---------------------------------------------- */
void WML_HistoryDeleteLeastUsed (pUA pUser);

/* ----------------------------------------------
	Sets the maximum number of allowed elements in
	the history list. (0 indicates unlimited.) When
	the number is changed, the new list is updated 
	to contain no more than the maximum number of
	allowed elements.
    Input: Actual User Agent, UINT16
	Output: -
   ---------------------------------------------- */
void WML_SetMaxInHistoryList (pUA pUser, UINT16 iMaxElements);

/* ----------------------------------------------
	Returns a array of pointers to the titles in
	the history list. NOTE! The titles are NOT
	copied, do not deallocate!
	Input: Actual User Agent
	Output: Pointer to WCHAR *, NULL if error.
   ---------------------------------------------- */
WCHAR **WML_GetHistoryListTitles (pUA pUser);

/* ============================================= */
/* ============================================= */





/* ------------ VAR TABLE FUNCTIONS ------------ 
	   Functions for handling the variables.
   --------------------------------------------- */
/* ----------------------------------------------
   Sets the value on the variable pchName to pchValue.
   STORES COPIES OF THE STRINGS!
   If the variable does not exist, it is created.
   The Name must be specified but the value may be
   NULL, in which the variable is removed. The 
   variable is set if there is enough memory and
   the name is a valid variable  name (consult WAP 
   WML section 10.3.1). If the value is equal to
   the empty string it is stored as NULL, i.e.
   removed.
   Input: Name (WCHAR *) and Value (WCHAR *)
   and the User Agent
   Output: TRUE if value is set, FALSE otherwise.					
   ---------------------------------------------*/
BOOL WML_SetVariable (pUA pUser, WCHAR *pchName, WCHAR *pchValue);

/* ----------------------------------------------
   Returns a COPY of the value of the variable with 
   the specified name.
   Input: Variable name and the User Agent
   Output: WCHAR *
   ---------------------------------------------*/
WCHAR *WML_GetVariable (pUA pUser, WCHAR *pchName);

/* ----------------------------------------------
   Deletes the variable table in the specified
   User Agent.
   Input: User Agent
   Output: -	
   ---------------------------------------------*/
void WML_EmptyVarTable (pUA pUser);

/* ----------------------------------------------
   If a navigation fails, the variables specified
   in the setvar elements in the task should not
   be set. These variables are stored temporary
   in a list in the user agent, that must be 
   deleted. This function removes that list. The
   function should be called whenever a task is
   failed.
   
   Input: User Agent
   Output: -
   ---------------------------------------------*/
void WML_DeleteEventVariables (pUA pUser);

/* ----------------------------------------------
   This function transfers the variables set in
   the task, from the temporary list in the user
   agent, to the variable list in the user agent
   (part of the context). The function also 
   removes the temporary list.
  
   Input: User Agent
   Output: -
   ---------------------------------------------*/
void WML_AddEventVariablesToContext (pUA pUser);

/* ----------------------------------------------
   Control the syntax of the variable name. Consult
   (consult WAP WML section 10.3.1)
   Input: Variable name
   Output: TRUE if OK, FALSE otherwise.	
   ---------------------------------------------*/
BOOL WML_CheckVariableSyntax (WCHAR *pchVariable);

/* ============================================= */
/* ============================================= */





/* ------------- SELECT FUNCTIONS -------------- 
	 Functions for handling the SELECT element.
   --------------------------------------------- */
/* ----------------------------------------------
	Sets the default value for a SELECT element.
	Input: Element of type SELECT and User Agent.
	Output: True if OK, FALSE otherwise.
   ---------------------------------------------- */
BOOL WML_SetSelectDefault (pELEMENTTYPE pSelect, pUA pUser);

/* ----------------------------------------------
	Updates the variables in a specified SELECT
	element according to the option elements in
	the content of the SELECT element. 
    Input: Element of type SELECT and User Agent.
	Output: True if OK, FALSE otherwise.
   ---------------------------------------------- */
BOOL WML_UpdateSelect (pELEMENTTYPE pSelect, pUA pUser);

/* ----------------------------------------------
	Updates all SELECT elements on a deck.
    Input: Element of type SELECT and User Agent.
	Output: True if OK, FALSE otherwise.
   ---------------------------------------------- */
BOOL WML_UpdateAllSelect (pELEMENTTYPE pCard, pUA pUser);

/* ----------------------------------------------
   Sets the 'SELECTED'-field in all the option 
   elements indicated in the NUMBERSTRUCT to TRUE, 
   all other to FALSE. 
   Input: Element of type OPTION or OPTGROUP, a
          NUMBERSTRUCT (NULL to deselect all), two
		  pointers to UINT16 (used internally, set
		  to 0). The ppDefaultOption pointer is 
		  also changed.
   Output: -
   ---------------------------------------------- */
void WML_PreSelectOptions(pELEMENTTYPE pElement, pNUMBERSTRUCT *ppDefaultOption,
					  UINT16 *piFindNbr, UINT16 *piThisNbr);

/* ============================================= */
/* ============================================= */





/* -------------- INPUT FUNCTIONS -------------- 
	 Functions for handling the INPUT element.
   --------------------------------------------- */
/* ----------------------------------------------
	Update the value of the Key of the specified
	INPUT Element. Stores a COPY of the value.
	Input: Element of type INPUT, Value and User
		   Agent.
	Output: True if OK, FALSE otherwise.
   ---------------------------------------------- */
BOOL WML_SetInputKeyValue (pUA pUser, pELEMENTTYPE pElement, WCHAR *pchValue);

/* ----------------------------------------------
	Returns a COPY of the Key value for the specified
	INPUT Element. If none, the default value is
	returned and the variable is set with the
	value.
    Input: Element of type INPUT and User Agent.
	Output: Pointer to WCHAR.
   ---------------------------------------------- */
WCHAR *WML_GetInputKeyValue (pUA pUser, pELEMENTTYPE pElement);

/* ============================================= */
/* ============================================= */





/* -------------- ACTION FUNCTIONS ------------- 
	   Functions for handling the different
	   actions and events.
   --------------------------------------------- */
/* ----------------------------------------------
	Updates the variables associated with a task.
	If the Action is of the type GO, PREV or REFRESH, 
	it can include VAR element.	These variables are 
	then set.    
	Input: Element and User Agent.
	Output: -
   ---------------------------------------------- */
void WML_UpdateEventVariables (pUA pUser, pELEMENTTYPE pAction);

/* ----------------------------------------------
	The function HandleEvent accepts an intrinsic
	event (eg. ONTIMER) and the ELEMENT (CARD)
	where it takes place. The appropriate action 
	is obtained, and is returned as an ELEMENT to 
	be executed. The function UpdateEventVariables
	is called if the fChangeVars is set, otherwise
	it is not called.
	Input: Actual User Agent, the element in which 
	       the event takes place (CARD), the type 
		   of the event and a boolean.
	Output: Pointer to an ELEMENT of one of the
			following types: PREV, REFRESH, NOOP,
			GO or TEXT (in the case of an URL).
			If no action, NULL is returned.
   ---------------------------------------------- */
pELEMENTTYPE WML_HandleEvent (pUA pUser, pELEMENTTYPE pElement, 
								 INT8 iEvent, BOOL fChangeVars);

/* ----------------------------------------------
    Returns the task for the specified element (DO,
	OPTION or A). The function UpdateEventVariables
	is called if the fChangeVars is set, otherwise
	it is not called.
    Input: Pointer to the User Agent the actual 
		   element and a boolean
	Output: Pointer to an ELEMENT of one of the
			following types: PREV, REFRESH, NOOP,
			GO or TEXT (in the case of an URL).
			If no action, NULL is returned.
   ---------------------------------------------- */
pELEMENTTYPE WML_HandleMMIAction (pUA pUser, pELEMENTTYPE pElement,
							  BOOL fChangeVars);

/* ============================================= */
/* ============================================= */


							  


/* -------------- TIMER FUNCTIONS -------------- 
	 Functions for handling the TIMER element.
   --------------------------------------------- */
/* ----------------------------------------------
    Returns the value of the timer.
    Input: Pointer to ELEMENT of type TIMER and
		   User Agent
	Output: UINT32
   ---------------------------------------------- */
UINT32 WML_GetTimer (pUA pUser, pELEMENTTYPE pTimer);

/* ----------------------------------------------
	Sets a new value to the variable specified by
	the timer element. The value is converted 
	from an UINT32 to a string (WCHAR *)
	Input: Pointer to ELEMENT of type TIMER,
		   User Agent and the new value (UINT32)
	Output: TRUE if OK, FALSE otherwise
   ---------------------------------------------- */
BOOL WML_StoreTimer (pUA pUser, pELEMENTTYPE pTimer, UINT32 iValue);

/* ============================================= */
/* ============================================= */




							  
/* ------------ ELEMENT FUNCTIONS -------------- 
	 Functions for handling the WML-elements.
   --------------------------------------------- */
/* ----------------------------------------------
    Returns the named CARD in the specified DECK.
	If the CARD is not found, the first card is 
	returned.
    Input: Pointer to User Agent and the Name of 
		   the Card
    Output: Pointer to ELEMENT of CARD, NULL if
			no card found.
   ---------------------------------------------- */
pELEMENTTYPE WML_GetCard (pUA pUser, WCHAR *pchName);

/* ----------------------------------------------
    Returns the next CARD for the specified User 
	Agent, and sets it to be the Current Card.
	If the current card is the last, the attribute
	pCurrentCard in pUser is not changed.
    Input: Pointer to the User Agent
    Output: Pointer to ELEMENT of CARD, NULL if 
			not found.
   ---------------------------------------------- */
pELEMENTTYPE WML_GetNextCard (pUA pUser);

/* ---------------------------------------------- 
	Returns a COPY of the title of the specified CARD.
	Input: Pointer to CARD and the User Agent
	Output: Pointer to WCHAR
   ---------------------------------------------- */
WCHAR *WML_GetCardTitle (pUA pUser, pELEMENTTYPE pCARD);

/* ---------------------------------------------- 
	Returns the first TEMPLATEDO element in the
	specified CARD
    Input: Pointer to CARD
	Output: Pointer to TEMPLATEDOTYPE, NULL if none.
   ---------------------------------------------- */
pTEMPLATEDOTYPE WML_GetTemplateDO (pELEMENTTYPE pCARD);

/* ---------------------------------------------- 
	Returns the next TEMPLATEDO element.
    Input: Pointer to TEMPLATEDO
	Output: Pointer to TEMPLATEDOTYPE
   ---------------------------------------------- */
pTEMPLATEDOTYPE WML_GetNextTemplateDO (pTEMPLATEDOTYPE pTemplateDO);

/* ----------------------------------------------
    Returns the pointer to the DO element in the 
	TEMPLATEDOTYPE struct.
	Input: Pointer to TEMPLATEDOTYPE
	Output: Pointer to ELEMENT
   ---------------------------------------------- */
pELEMENTTYPE WML_GetDOFromTemplate (pTEMPLATEDOTYPE pTemplateDO);

/* ----------------------------------------------
	Examines if a p element is insignificant 
	(NULL, empty or just white space).

	Input: User agent and PELEMENT
	Output: TRUE if insignificant, FALSE otherwise
   ---------------------------------------------- */
BOOL WML_IsPEmpty (pUA pUser, pELEMENTTYPE pPElement);

/* ============================================= */
/* ============================================= */





/* ------------- VARIOUS FUNCTIONS ------------- 
	 Functions for handling the WML-structure.
   --------------------------------------------- */
/* ----------------------------------------------
    Returns a COPY of the text in an ELEMENTTYPE
	of type text with the variables set.
	Input: Pointer to ELEMENTTYPE of Type_Text and
	       a User Agent (NOT NULL)
	Output: Pointer to WCHAR or NULL.
   ---------------------------------------------- */
WCHAR *WML_GetString (pUA pUser, pELEMENTTYPE pElement);

/* ----------------------------------------------
    Returns the length of the text in an ELEMENTTYPE
	of type text with the variables set. (Excluding 
	termination character).
	Input: Pointer to ELEMENTTYPE of Type_Text and
	       a User Agent (NOT NULL)
	Output: UINT32 
   ---------------------------------------------- */
UINT32 WML_GetStringLength (pUA pUser, pELEMENTTYPE pElement);

/* ----------------------------------------------
    Returns a COPY of the text in a TEXTELEMENT
	with the variables set.
	Input: Pointer to TEXTELEMENT and a User Agent
		   (NOT NULL)
	Output: Pointer to WCHAR or NULL.
   ---------------------------------------------- */
WCHAR *WML_GetTextString (pUA pUser, pTEXTELEMENT pElement);

/* ----------------------------------------------
    Returns the length of the text in a TEXTELEMENT
	with the variables set. (Excluding termination 
	character).
	Input: Pointer to TEXTELEMENT and a User Agent
		   (NOT NULL)
	Output: UINT32 
   ---------------------------------------------- */
UINT32 WML_GetTextStringLength (pUA pUser, pTEXTELEMENT pElement);

/* ----------------------------------------------
    Removes all insignificant white space in a
    null-terminated text. The text is modified.
    The iMode parameter specifies which of the 
    leading and trailing white space that should 
    be removed. (E.g., if LEFT is specified, all 
    leading white space are removed.)
    Input: WCHAR * or BYTE *
    Output: TRUE if text ends with a white space,
			FALSE otherwise
   ---------------------------------------------- */
BOOL WML_TrimTextWCHAR (WCHAR *pchText, UINT8 iMode);
BOOL WML_TrimTextBYTE (BYTE *pbText, UINT8 iMode);

/* ----------------------------------------------
	Returns the actual text encoding format
	(UTF-8 or UCS_2) in IANA code.
	Input: Pointer to User Agent
	Output: Integer (consult PortingGuide)
   ---------------------------------------------- */
INT16 WML_GetTextEncodForm (pUA pUser);

/* ----------------------------------------------
	Checks the ACCESS element against an URL. If
	the check is OK, TRUE is returned, otherwise
	FALSE is returned.
    Input: ACCESS element, URL (BYTE *)
	Output: BOOL
   ---------------------------------------------- */
BOOL WML_CheckAccessElement (pELEMENTTYPE pAccess, BYTE *pbURL,
								 BYTE *pbDefaultDomain);

/* ----------------------------------------------
	This function empties the variable table and 
	removes all history entries but the newest.
	Input: User agent
	Output: -
   ---------------------------------------------- */
void WML_NewContext (pUA pUser);


/* ----------------------------------------------
	Script function
	Input: -
	Output: -
   ---------------------------------------------- */
void UApragmaFromScript( pUA userAgent, WCHAR** ppwchPropertyName, 
		WCHAR** ppwchContent, WCHAR** ppwchScheme );

/* ============================================= */
/* ============================================= */

#endif
