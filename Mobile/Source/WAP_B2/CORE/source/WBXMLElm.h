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
	FILE: WBXMLElm.h
  
	General WBXML-decoder. Element handler.	Interface file.

	Rev history:
		990302	JPR		Created
		990316	JPR		Modified
		990915	JPR		Additions to support channel decoding
		991112	JPR		Updated to support PUSH content types

   ================================================================== */

#ifndef WBXMLELM_H
#define WBXMLELM_H

#include "WBXMLDef.h"
#include "WMLElm.h"

/* WTA specific */
#ifdef CONFIG_WTA
	#include "WTAChElm.h"
#endif

/* PUSH specific */
#ifdef CONFIG_PUSH
	#include "PUSHElm.h"
#endif

/* -----------------------------------------------
   Creates a COPY of the specified string.
   Input: WCHAR *
   Output: WCHAR *, NULL if error
   ----------------------------------------------- */
WCHAR *CreateStringCopy (WCHAR *pchString);


/* =============================================== */
/* =============================================== */
/*					TEXT FUNCTIONS				   */
/* =============================================== */
/* =============================================== */

/* -----------------------------------------------
   Creates a new TEXTELEMENT.
   Input: -
   Output: Pointer to TEXTELEMENT, NULL if error. 
   ----------------------------------------------- */
pTEXTELEMENT Text_CreateText (void);

void Text_DeleteText (pTEXTELEMENT *ppText);

/* -----------------------------------------------
   Functions for handling the text. If the data
   cannot be added it is deleted from memory.
   Input: Pointer to pointer to TEXTELEMENT and 
		  the data.
   Output: TRUE if success, FALSE otherwise			
   ----------------------------------------------- */
BOOL Text_AddString (pTEXTELEMENT *ppText, WCHAR *pchTextData);
BOOL Text_AddHeadString (pTEXTELEMENT *ppText, WCHAR *pchTextData);
BOOL Text_AddEntity (pTEXTELEMENT *ppText, UINT32 iEntity);
BOOL Text_AddExtension (pTEXTELEMENT *ppText, WCHAR *pchData, enum TextType iExtType);

/* -----------------------------------------------
	Computes the length for a text, including 
	entities, but not the extensions.
	Input: Pointer to TEXTELEMENT.
	Output: Total length (excluding termination 
			characters)
   ------------------------------------------------ */
UINT32 Text_GetLength (pTEXTELEMENT pText);

/* -----------------------------------------------
	Converts the entities and the strings in a
	text element into a string. The text element
	is left unchanged
	Input: Pointer to TEXTELEMENT.
	Output: Pointer to WCHAR *, NULL if error.
   ------------------------------------------------ */
WCHAR *Text_GetText (pTEXTELEMENT pText);

/* -----------------------------------------------
	Converts the entities and the strings in a
	text element into a string. NOTE! The text 
	element	is always discarded.
	Input: Pointer to TEXTELEMENT.
	Output: Pointer to WCHAR *, NULL if error.
   ------------------------------------------------ */
WCHAR *Text_ToString (pTEXTELEMENT *ppText);

void Text_AppendText (pTEXTELEMENT *ppText, pTEXTELEMENT pNewText);

/* ----------------------------------------------
	Determines what kind of text a token is.
	Input: BYTE
	Output: Kind of text, T_String is default.
   ---------------------------------------------- */
enum TextType WBXML_GetTextType (BYTE bByte);


/* =============================================== */
/* =============================================== */
/*				ELEMENT FUNCTIONS				   */
/* =============================================== */
/* =============================================== */

/* ----------------------------------------------
   Creates an element of the specified type and 
   sets the default values.
   Input: ElmType, DTD, and ViewID								
   Output: Pointer to an Element, NULL if error
   ----------------------------------------------*/
pELEMENTTYPE XML_CreateElement (UINT8 iType, UINT8 iDTD, UINT8 iViewID);

/* ----------------------------------------------
   Deletes an element and frees the memory.
   Input: Pointer to an element and DTD	
   Output: -
   ----------------------------------------------*/
void XML_DeleteElement (pELEMENTTYPE pElement, UINT8 iDTD);

/* -----------------------------------------------
   Creates a new ELEMENTTYPE of type Type_Text and
   adds a TEXTELEMENT. If error, everything is
   deleted.
   Input: Pointer to TEXTELEMENT
   Output: Pointer to ELEMENTTYPE, NULL if error. 
   ----------------------------------------------- */
pELEMENTTYPE XML_CreateTextElement (pTEXTELEMENT *ppText);

/* ----------------------------------------------
   Add the specified content to an element.
   Input: Element, content and DTD
   Output: -
   ----------------------------------------------*/
void XML_AddContent (pELEMENTTYPE pElement, pELEMENTTYPE pContent, UINT8 iDTD);

/* ----------------------------------------------
    Returns the first element in the content for 
	the specified element.
	Input: Pointer to ELEMENT and DTD
	Output: Pointer to ELEMENT, NULL if no content.
   ---------------------------------------------- */
pELEMENTTYPE XML_GetContent (pELEMENTTYPE pElement, UINT8 iDTD);

/* ----------------------------------------------
    Returns the next element on the same level.
	Input: Pointer to ELEMENT
	Output: Pointer to ELEMENT, NULL if no more 
			elements
   ---------------------------------------------- */
pELEMENTTYPE XML_GetNextElement (pELEMENTTYPE pElement);

/* =============================================== */
/* =============================================== */


#endif
