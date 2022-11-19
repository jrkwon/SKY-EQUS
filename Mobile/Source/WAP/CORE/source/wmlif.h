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
		991124	JPR		History functions moved to History.h

		991207	JPR		Changes to support new generic WBXML-decoder

==========================================================================*/

#ifndef _INTERFACEH
#define _INTERFACEH

#include "wmldef.h"

typedef struct tNUMBERSTRUCT
{
	UINT16 iIndex;
	struct tNUMBERSTRUCT *pNext;
} NUMBERSTRUCT, *pNUMBERSTRUCT;


/*========================================================================

	Functions for decoding wmlc-content

=========================================================================

	Functions used to parse a WML-deck. The functions uses the general 
	WBXML-decoder (WBXMLDec) and for the WML-specific parts it uses 
	WMLParse.

	The functions must be used in the following manner:

	1. An user agent is created with the WML_CreateUserAgent function. 
	   The user agent contains information of the wml-context and is used 
	   until it is terminated.

	2. For each wml-deck to be parsed, the function WML_StartParser is 
	   called initially. If it returns TRUE step 3 should be executed. 
	   Otherwise, step 4 should be executed.

	3. The function WML_ParseNextElement is called repeatedly until NULL 
	   is returned which indicates that the whole deck has been parsed.

	4. The function WML_EndParser is called if step (3) is completed. The 
	   function returns TRUE if the  parse was successful, FALSE otherwise. 
	   If step (3) is interrupted earlier, the function WML_AbortParser 
	   must be called instead.
	
	5. The functions WML_EmptyDECK, WML_EmptyVarTable, DeleteHostList may 
	   be called to alter the context of the user agent.

	6. Repeat step (2-5) for each new deck to be parsed.

    7. Delete the user agent with the function WML_DeleteUserAgent. The 
	   function also removes the variable table, the current deck, the 
	   history list, the decode struct, the HostList, and the URL Base.

==========================================================================*/

/*========================================================================
	WML_CreateUserAgent
==========================================================================
	Creates an User Agent and sets the attributes to their default values.
	
	Input: -
	Output: Pointer to UA, NULL if error
==========================================================================*/
pUA WML_CreateUserAgent (void);


/*========================================================================
	WML_DeleteUserAgent
==========================================================================
	Deletes an User Agent and removes the attributes pbURLBase, 
	pVisitedHosts, the variable table, the string table, the decode struct 
	and the current deck.
	
	Input: **UA
	Output: -
==========================================================================*/
void WML_DeleteUserAgent (pUA *ppUser);


/*========================================================================
	WML_StartParser
==========================================================================
    Starts up the parser. Parses the prolog and builds the string table. 
	During this phase the version number (WBXML) is also verified. 
	
	Input: User Agent, pointer to bytestream, the number of bytes in the 
		   bytestream, the content type and content level.
	Output: TRUE if OK, FALSE otherwise.
==========================================================================*/
BOOL WML_StartParser (pUA pUser, BYTE *pbStream, UINT32 iNbrOfBytes,
					  BYTE bContentType, UINT8 iContentLevel); 


/*========================================================================
	WML_EndParser
==========================================================================
    Finishes the parse of a deck. Removes the temporary variables. Stores 
	all not overridden TEMPLATE DO elements in the cards. If the parse
	was successful TRUE is returned, otherwise FALSE is returned. 

	If the function returns FALSE, the string table and the deck are 
	deleted.
	
	Input: User Agent
	Output: TRUE if OK, FALSE otherwise.
==========================================================================*/
BOOL WML_EndParser (pUA pUser);


/*========================================================================
	WML_AbortParser
==========================================================================
    Aborts the parse of a deck. Removes the temporary variables, and the 
	string table.
	
	Input: User Agent
	Output: -
==========================================================================*/
void WML_AbortParser (pUA pUser);


/*========================================================================
	WML_ParseNextElement
==========================================================================
    Parses the wml content elements in the deck, one by one. CARD, HEAD 
	and TEMPLATE are at this level. The elements are stored in the User 
	Agent. The function should be called repeteadly until it returns
	FALSE.
	
	Input: User Agent
	Output: TRUE if the function should be called again, FALSE otherwise
==========================================================================*/
BOOL WML_ParseNextElement (pUA pUser);


/*========================================================================
	WML_EmptyDECK
==========================================================================
	Deletes the WML deck and the string table in the specified User Agent
	
	Input: User Agent
	Output: -
==========================================================================*/
void WML_EmptyDECK (pUA pUser);




/*========================================================================

	VARIABLE TABLE FUNCTIONS

=========================================================================*/

/*========================================================================
	WML_SetVariable 
==========================================================================
	Sets the value on the variable pchName to pchValue. STORES COPIES OF 
	THE STRINGS! If the variable does not exist, it is created. The Name 
	must be specified but the value may be NULL, in which the variable is 
	removed. The variable is set if there is enough memory and the name is 
	a valid variable name (consult WAP WML 1.2 section 10.3.1). If the 
	value is equal to the empty string it is stored as NULL, i.e. removed.

	If the fWTA-flag is set, WTA-event parameters are also allowed (i.e.,
	variables starting with '1' - '9', consult WTA specification for more
	information).
  
	Input: Name (WCHAR *) and Value (WCHAR *), User Agent and WTA-flag
	Output: TRUE if value is set, FALSE otherwise. 
==========================================================================*/
BOOL WML_SetVariable (pUA pUser, WCHAR *pchName, WCHAR *pchValue, BOOL fWTA);


/*========================================================================
	WML_GetVariable 
==========================================================================
	Returns a COPY of the value of the variable with the specified name.
	
	Input: Variable name and the User Agent
	Output: WCHAR *, NULL if not found
==========================================================================*/
WCHAR* WML_GetVariable (pUA pUser, WCHAR *pchName);


/*========================================================================
	WML_EmptyVarTable
==========================================================================
	Deletes the variable table in the specified User Agent.

	Input: User Agent
	Output: -
==========================================================================*/
void WML_EmptyVarTable (pUA pUser);


/*========================================================================
	WML_DeleteEventVariables
==========================================================================
	If a navigation fails, the variables specified in the setvar elements 
	in the task should not be set. These variables are stored temporary
	in a list in the user agent, that must be deleted. This function 
	removes that list. The function should be called whenever a task is
	failed.

	Input: User Agent
	Output: -
==========================================================================*/
void WML_DeleteEventVariables (pUA pUser);


/*========================================================================
	WML_AddEventVariablesToContext
==========================================================================
	This function transfers the variables set in the task, from the 
	temporary list in the user agent, to the variable list in the user 
	agent (part of the context). The function also removes the temporary 
	list.

	Input: User Agent
	Output: -
==========================================================================*/
void WML_AddEventVariablesToContext (pUA pUser);


/*========================================================================
	WML_CheckVariableSyntax
==========================================================================
	This function checks the syntax of the variable name. If the variable
	name is valid TRUE isr returned, otherwise FALSE is returned. (Consult 
	WAP WML 1.2 section 10.3.1 for more information.)

	If the fWTA-flag is set, WTA-event parameters are also allowed (i.e.,
	variables starting with '1' - '9', consult WTA specification for more
	information).

	Input: Variable name and WTA-flag
	Output: TRUE if OK, FALSE otherwise.	
==========================================================================*/
BOOL WML_CheckVariableSyntax (WCHAR *pchVariable, BOOL fWTA);




/*========================================================================

	SELECT ELEMENT FUNCTIONS

=========================================================================*/

/*========================================================================
	WML_SetSelectDefault
==========================================================================
	Sets the default value for a SELECT element.

	Input: Element of type SELECT and User Agent.
	Output: True if OK, FALSE otherwise.
==========================================================================*/
BOOL WML_SetSelectDefault (pELEMENTTYPE pSelect, pUA pUser);


/*========================================================================
	WML_UpdateSelect
==========================================================================
	Updates the variables in a specified SELECT element according to the 
	option elements in the content of the SELECT element. 

	Input: Element of type SELECT and User Agent.
	Output: True if OK, FALSE otherwise.
==========================================================================*/
BOOL WML_UpdateSelect (pELEMENTTYPE pSelect, pUA pUser);


/*========================================================================
	WML_UpdateAllSelect
==========================================================================
	Updates all SELECT elements on a card.

	Input: Element of type CARD and User Agent.
	Output: True if OK, FALSE otherwise.
==========================================================================*/
BOOL WML_UpdateAllSelect (pELEMENTTYPE pCard, pUA pUser);


/*========================================================================
	WML_PreSelectOptions
==========================================================================
	Sets the 'SELECTED'-field in all the option elements indicated in the 
	NUMBERSTRUCT to TRUE, all other to FALSE. 

	Input: Element of type OPTION or OPTGROUP, a NUMBERSTRUCT (NULL to 
		   deselect all), two pointers to UINT16 (used internally, set
		   to 0). The ppDefaultOption pointer is also changed.
	Output: -
==========================================================================*/
void WML_PreSelectOptions(pELEMENTTYPE pElement, pNUMBERSTRUCT *ppDefaultOption,
						  UINT16 *piFindNbr, UINT16 *piThisNbr);





/*========================================================================

	INPUT ELEMENT FUNCTIONS

=========================================================================*/

/*========================================================================
	WML_SetInputKeyValue
==========================================================================
	Update the value of the Key of the specified INPUT Element. Stores a 
	COPY of the value.

	Input: Element of type INPUT, Value and User Agent.
	Output: True if OK, FALSE otherwise.
==========================================================================*/
BOOL WML_SetInputKeyValue (pUA pUser, pELEMENTTYPE pElement, WCHAR *pchValue);


/*========================================================================
	WML_SetInputKeyValue
==========================================================================
	Returns a COPY of the Key value for the specified INPUT Element. If 
	none, the default value is returned and the variable is set with the
	value.

	Input: Element of type INPUT, Value and User Agent.
	Output: Pointer to WCHAR.
==========================================================================*/
WCHAR* WML_GetInputKeyValue (pUA pUser, pELEMENTTYPE pElement);





/*========================================================================

	ACTION ELEMENT FUNCTIONS

=========================================================================*/

/*========================================================================
	WML_UpdateEventVariables
==========================================================================
	Updates the variables associated with a task. If the Action is of the 
	type GO, PREV or REFRESH, it can include VAR element. These variables 
	are then set.    

	Input: Element and User Agent.
	Output: -
==========================================================================*/
void WML_UpdateEventVariables (pUA pUser, pELEMENTTYPE pAction);


/*========================================================================
	WML_HandleEvent
==========================================================================
	The function HandleEvent accepts an intrinsic event (eg. ONTIMER) and 
	the ELEMENT (CARD) where it takes place. The appropriate action is 
	obtained, and is returned as an ELEMENT to be executed. The function 
	UpdateEventVariables is called if the fChangeVars is set, otherwise
	it is not called.

	Input: Actual User Agent, the element in which the event takes place 
		   (CARD), the type of the event and a boolean.
	Output: Pointer to an ELEMENT of one of the following types: PREV, 
			REFRESH, NOOP, GO or TEXT (in the case of an URL). If no 
			action, NULL is returned.
==========================================================================*/
pELEMENTTYPE WML_HandleEvent (pUA pUser, pELEMENTTYPE pElement, INT8 iEvent, 
							  BOOL fChangeVars);

/*========================================================================
	WML_HandleMMIAction
==========================================================================
    Returns the task for the specified element (DO, OPTION or A). The 
	function UpdateEventVariables is called if the fChangeVars is set, 
	otherwise it is not called.

	Input: Pointer to the User Agent the actual element and a boolean
	Output: Pointer to an ELEMENT of one of the following types: PREV, 
			REFRESH, NOOP, GO or TEXT (in the case of an URL). If no 
			action, NULL is returned.
==========================================================================*/
pELEMENTTYPE WML_HandleMMIAction (pUA pUser, pELEMENTTYPE pElement, BOOL fChangeVars);



							  


/*========================================================================

	TIMER ELEMENT FUNCTIONS

=========================================================================*/

/*========================================================================
	WML_GetTimer
==========================================================================
    Returns the value of the timer element.

	Input: Pointer to ELEMENT of type TIMER and User Agent
	Output: UINT32
==========================================================================*/
UINT32 WML_GetTimer (pUA pUser, pELEMENTTYPE pTimer);


/*========================================================================
	WML_StoreTimer
==========================================================================
	Sets a new value to the variable specified by the timer element. The 
	value is converted from an UINT32 to a string (WCHAR *)

	Input: Pointer to ELEMENT of type TIMER, User Agent and the new value 
		   (UINT32)
	Output: TRUE if OK, FALSE otherwise
==========================================================================*/
BOOL WML_StoreTimer (pUA pUser, pELEMENTTYPE pTimer, UINT32 iValue);





							  
/*========================================================================

	WML ELEMENT FUNCTIONS 

=========================================================================*/

/*========================================================================
	WML_GetNextElement
==========================================================================
	The function returns the next element on the same level as the 
	specified element. 

	NOTE! All concecutive Type_Inline_Text, Type_Table_Text, Type_Entity,
	and Type_Ext are treated as ONE element to conform with the 
	WML_GetString and WML_GetStringLength functions.

	Input: ELEMENTTYPE*
	Output: ELEMENTTYPE* or NULL
==========================================================================*/
pELEMENTTYPE WML_GetNextElement (pELEMENTTYPE pElement);


/*========================================================================
	WML_GetCard
==========================================================================
    Returns the named CARD in the specified DECK. If the CARD is not 
	found, the first card is returned.

	Input: Pointer to User Agent and the Name of the Card
	Output: Pointer to ELEMENT of CARD, NULL if	no card found.
==========================================================================*/
pELEMENTTYPE WML_GetCard (pUA pUser, WCHAR *pchName);


/*========================================================================
	WML_GetNextCard
==========================================================================
    Returns the next CARD for the specified User Agent, and sets it to be 
	the Current Card. If the current card is the last, the attribute
	pCurrentCard in pUser is not changed.

	Input: Pointer to the User Agent
	Output: Pointer to ELEMENT of CARD, NULL if not found.
==========================================================================*/
pELEMENTTYPE WML_GetNextCard (pUA pUser);


/*========================================================================
	WML_GetCardTitle
==========================================================================
	Returns a COPY of the title of the specified CARD.

	Input: Pointer to CARD and the User Agent
	Output: Pointer to WCHAR
==========================================================================*/
WCHAR* WML_GetCardTitle (pUA pUser, pELEMENTTYPE pCARD);


/*========================================================================
	WML_GetTemplateDO
==========================================================================
	Returns the first TEMPLATEDO element in the specified CARD.

	Input: Pointer to CARD
	Output: Pointer to TEMPLATEDOTYPE, NULL if none.
==========================================================================*/
pTEMPLATEDOTYPE WML_GetTemplateDO (pELEMENTTYPE pCARD);


/*========================================================================
	WML_GetNextTemplateDO
==========================================================================
	Returns the next TEMPLATEDO element.

	Input: Pointer to TEMPLATEDO
	Output: Pointer to TEMPLATEDOTYPE, NULL if no more.
==========================================================================*/
pTEMPLATEDOTYPE WML_GetNextTemplateDO (pTEMPLATEDOTYPE pTemplateDO);


/*========================================================================
	WML_GetDOFromTemplate
==========================================================================
    Returns the pointer to the DO element in the TEMPLATEDOTYPE struct.

	Input: Pointer to TEMPLATEDO
	Output: Pointer to ELEMENT.
==========================================================================*/
pELEMENTTYPE WML_GetDOFromTemplate (pTEMPLATEDOTYPE pTemplateDO);


/*========================================================================
	WML_CheckAccessElement
==========================================================================
	Checks the ACCESS element against an URL. If the check is OK, TRUE is 
	returned, otherwise FALSE is returned.

	Input: ACCESS element, URL (BYTE *)
	Output: TRUE/FALSE
==========================================================================*/
BOOL WML_CheckAccessElement (pELEMENTTYPE pAccess, BYTE *pbURL, BYTE *pbDefaultDomain);


/*========================================================================
	WML_NewContext
==========================================================================
	This function empties the variable table and removes all history 
	entries but the newest.

	Input: User agent
	Output: -
==========================================================================*/
void WML_NewContext (pUA pUser);


/*========================================================================
	UApragmaFromScript
==========================================================================
	Script function

	Input: -
	Output: -
==========================================================================*/
void UApragmaFromScript(pUA userAgent, WCHAR** ppwchPropertyName, 
						WCHAR** ppwchContent, WCHAR** ppwchScheme );


/*========================================================================
	WML_IsPEmpty
==========================================================================
	Examines if a p element is insignificant  (NULL, empty or just white 
	space).

	Input: User agent and PELEMENT
	Output: TRUE if insignificant, FALSE otherwise
==========================================================================*/
BOOL WML_IsPEmpty (pUA pUser, pELEMENTTYPE pPElement);


/*========================================================================
	WML_TrimTextWCHAR
	WML_TrimTextBYTE
==========================================================================
    Removes all insignificant white space in a null-terminated text. The 
	text is modified. The iMode parameter specifies which of the leading 
	and trailing white space that should be removed. (E.g., if LEFT is 
	specified, all leading white space are removed.)

	Input: WCHAR* / BYTE*
	Output: TRUE if text ends with a white space, FALSE otherwise
==========================================================================*/
BOOL WML_TrimTextWCHAR (WCHAR *pchText, UINT8 iMode);
BOOL WML_TrimTextBYTE (BYTE *pbText, UINT8 iMode);


/*========================================================================
	WML_GetTextEncodForm 
==========================================================================
	Returns the actual text encoding format (UTF-8 or UCS_2) in IANA code.

	Input: Pointer to User Agent
	Output: Integer (consult PortingGuide)
==========================================================================*/
INT16 WML_GetTextEncodForm (pUA pUser);


/*========================================================================
	WML_GetString
==========================================================================
    The function returns a COPY of the string created of all concecutive 
	elements of the type Type_Inline_Text, Type_Table_Text, Type_Entity,
	or Type_Ext. (The variables in the string are set)

	Input: Pointer to ELEMENTTYPE of type Type_Inline_Text, Type_Entity,
		   Type_Table_Text, or Type_ExtType_Text and a User Agent
	Output: WCHAR* or NULL
==========================================================================*/
WCHAR *WML_GetString (pUA pUser, pELEMENTTYPE pElement);


/*========================================================================
	WML_GetStringLength
==========================================================================
    The function returns the length of the string created of all 
	concecutive elements of the type Type_Inline_Text, Type_Table_Text, 
	Type_Entity, or Type_Ext. (The variables in the string are set)

	Input: Pointer to ELEMENTTYPE of type Type_Inline_Text, Type_Entity,
		   Type_Table_Text, or Type_ExtType_Text and a User Agent
	Output: UINT32
==========================================================================*/
UINT32 WML_GetStringLength (pUA pUser, pELEMENTTYPE pElement);


#endif
