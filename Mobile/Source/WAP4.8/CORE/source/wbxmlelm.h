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
	FILE: wbxmlelm.h
  
	General WBXML-decoder. Element handler.	Interface file.

	Rev history:
		990302	JPR		Created
		990316	JPR		Modified
		990915	JPR		Additions to support channel decoding
		991112	JPR		Updated to support PUSH content types

  		991125	JPR		Redesign of Element Handling, Text Handling,
						and Error Handling (Consult separate document).
						Application specific parts removed
		991129	JPR		Function XML_AddHeadString added
		991202	JPR		FindStringBinarySearch and functions for creating 
						and deleting empty elements added.
		000131	JPR		Functions XML_AddContent and XML_GetContent added.
		010119	NKE		Replaced WML_AddContent with WML_ValidContent
						for the new decoder in wbxmldec.c.

==========================================================================*/

#ifndef WBXMLELM_H
#define WBXMLELM_H

#include "wbxmldef.h"

/*========================================================================
	CreateStringCopy
==========================================================================
	Creates a COPY of the specified string. It is the caller's responsi-
	bility to deallocate the memory.

	Input: WCHAR*
	Output: WCHAR* (NULL if error and if HAS_SETJMP not set)
==========================================================================*/
WCHAR *CreateStringCopy (WCHAR *pchString);


/*========================================================================
	FindStringBinarySearch
==========================================================================
	The function searches an array of stings (WCHAR*) for the specified 
	string. If the string is found the index (0..n) is returned. If the
	string is not found -1 is returned.

  	NOTE! The array of strings MUST be sorted!

	Input: String, String table, and table length.
	Output: Index of string in table, -1 if not found or error
==========================================================================*/
INT16 FindStringBinarySearch (const WCHAR* pwchString, 
							  const WCHAR* const ppwchStringTab[], UINT16 iTabLen);



/*========================================================================

	Text Element functions

=========================================================================*/

/*========================================================================
	XML_GetStringLength
==========================================================================
	The function computes the total number of characters of the string of 
	all consecutive Type_Text elements and Type_Entity elements starting
	with the element in the parameter. The termination character is NOT 
	included. Each entity is counted as one character.
	
	Input: Starting element (Type_Inline_Text, Type_Table_Text or 
		   Type_Entity), and pWBXMLDECODESTR (not NULL)
	Output: Length of string (UINT32), not including termination character
==========================================================================*/
UINT32 XML_GetStringLength (pELEMENTTYPE pStartElement, pWBXMLDECODESTR pDecStr);


/*========================================================================
	XML_GetString
==========================================================================
	The function creates a WCHAR-string from all consecutive Type_Text 
	elements and Type_Entity elements starting with the element in the 
	parameter. The XML elements are left unchanged. The string is null 
	terminated and it is the caller's responsibility to deallocate it 
	after it is used.
	
	Input: Starting element (Type_Inline_Text, Type_Table_Text or 
		   Type_Entity), and pWBXMLDECODESTR (not NULL)
	Output: WCHAR*, or NULL
==========================================================================*/
WCHAR* XML_GetString (pELEMENTTYPE pStartElement, pWBXMLDECODESTR pDecStr);


/*========================================================================
	XML_ConvertToString
==========================================================================
	The function creates a WCHAR-string from all consecutive Type_Text 
	elements and Type_Entity elements starting with the element in the 
	parameter. The XML elements are then deleted and the in-parameter is
	set to point to the first following element not used in the string. 
	
	The returned string is null terminated and it is the caller's 
	responsibility to deallocate it after it is used.
	
	Input: Starting element (Type_Inline_Text, Type_Table_Text or 
		   Type_Entity), and pWBXMLDECODESTR (not NULL)
	Output: WCHAR*, or NULL
==========================================================================*/
WCHAR* XML_ConvertToString (pELEMENTTYPE* ppStartElement, pWBXMLDECODESTR pDecStr);


/*========================================================================
	XML_AddHeadString
==========================================================================
	The function creates a WCHAR-string from the inputted string (CHAR*)
	and stores the result in a Type_Inline_Text element. The element is
	then stored at the beginning of the element list (ppStartElement).

	If HAS_SETJMP is not set and the client runs out of memory, the 
	iDecodeResult integer is set accordingly.
		
	Input: Starting element (Type_Inline_Text, Type_Table_Text or 
		   Type_Entity), the string (not altered) and pWBXMLDECODESTR 
		   (not NULL)
	Output: -
==========================================================================*/
void XML_AddHeadString (pELEMENTTYPE* ppStartElement, const CHAR* pchString,
						pWBXMLDECODESTR pDecStr);



/*========================================================================

	Functions for handling Global Elements

=========================================================================*/

/*========================================================================
	XML_CreateEmptyElement
==========================================================================
	The function creates an element with no attributes or contents. The
	new element of gets the type indicated by the iType parameter. 
	
	NOTE! Elements created with this function MUST be deleted by the 
	XML_DeleteEmptyElement function.

	Input: Type of element (UINT16)
	Output: Element or NULL
==========================================================================*/
pELEMENTTYPE XML_CreateEmptyElement (UINT16 iType);


/*========================================================================
	XML_DeleteEmptyElement
==========================================================================
	The function creates a new element created with the function
	XML_CreateEmptyElement.
	
	Input: ELEMENTTYPE**
	Output: -
==========================================================================*/
void XML_DeleteEmptyElement (pELEMENTTYPE* ppElement);


/*========================================================================
	XML_AddContent
==========================================================================
	The function adds the content to the element. The pElement is first
	casted to 'Type_ContentElement' and the content is then stored.

	NOTE! The element MUST be conformant with the tCONTENTELEMENT element
	definition, i.e., content as its third attribute in the struct.

	NOTE! The Element MUST NOT be NULL.
	
	Input: Element (ELEMENTTYPE*) and Content (ELEMENTTYPE**)
	Output: -
==========================================================================*/
void XML_AddContent (pELEMENTTYPE pElement, pELEMENTTYPE* ppContent);


/*========================================================================
	XML_GetContent
==========================================================================
	The function retrieves the content in an element. The pElement is 
	first casted to 'Type_ContentElement' and the content is returned.

	NOTE! The element MUST be conformant with the tCONTENTELEMENT element
	definition, i.e., content as its third attribute in the struct.

	NOTE! The Element MUST NOT be NULL.
	
	Input: Element (ELEMENTTYPE*)
	Output: Content (ELEMENTTYPE*), or NULL
==========================================================================*/
pELEMENTTYPE XML_GetContent (pELEMENTTYPE pElement);


/*========================================================================
	XML_CreateInlineText
==========================================================================
	The function creates a new element of type Type_Inline_Text and stores 
	the	specified string in it. If error, the ppwchString is deallocated.
	
	Input: Null-terminated string (WCHAR**)
	Output: Element or NULL
==========================================================================*/
pELEMENTTYPE XML_CreateInlineText (WCHAR** ppwchString);


/*========================================================================
	XML_CreateTableText
==========================================================================
	The function creates a new element of type Type_Table_Text and stores 
	the	specified table reference in it.
	
	Input: String table reference (UINT32)
	Output: Element or NULL
==========================================================================*/
pELEMENTTYPE XML_CreateTableText (UINT32 iTableRef);


/*========================================================================
	XML_DeleteText
==========================================================================
	The function deletes an element of type Type_Inline_Text or 
	Type_Table_Text and all data in it.

	The in-parameter ppText is set to point to NULL after the element has 
	been deleted. If the element is not of one of the above types, the
	element is left unchanged.

	Input: ELEMENTTYPE** of type Type_Inline_Text or Type_Table_Text
	Output: -
==========================================================================*/
void XML_DeleteText (pELEMENTTYPE* ppText);


/*========================================================================
	XML_CreateExt
==========================================================================
	This function is used to create an extension (EXT) element.
	
	The function creates a new element of type Type_Ext_0, Type_Ext_1, or
	Type_Ext_2 and stores the specified string in it. If error, the string 
	is deallocated.
	
	Input: WCHAR** and the type of the extension.
	Output: Element or NULL
==========================================================================*/
pELEMENTTYPE XML_CreateExt (WCHAR** ppwchString, BYTE bType);


/*========================================================================
	XML_DeleteExt
==========================================================================
	This function is used to delete any extension element.
	
	The in-parameter ppExt is set to point to NULL after the element has 
	been deleted. If the element is not an extension, the element is left 
	unchanged.	

	Input: ELEMENTTYPE** of any extension type
	Output: -
==========================================================================*/
void XML_DeleteExt (pELEMENTTYPE* ppExt);


/*========================================================================
	XML_CreateEntity
==========================================================================
	The function creates a new element of type Type_Entity and stores the
	specified entity in it.
	
	Input: UINT32 specifying the entity 
	Output: Element or NULL
==========================================================================*/
pELEMENTTYPE XML_CreateEntity (UINT32 iEntity);


/*========================================================================
	XML_DeleteEntity
==========================================================================
	The function deletes an element of type Type_Entity and all data in it.

	The in-parameter ppEntity is set to point to NULL after the element 
	has been deleted. If the element is not an entity, the element is left 
	unchanged.
  
	Input: ELEMENTTYPE** of type Type_Entity
	Output: -
==========================================================================*/
void XML_DeleteEntity (pELEMENTTYPE* ppEntity);


/*========================================================================
	XML_CreateOpaque
==========================================================================
	The function creates a new element of type Type_Opaque and stores the
	specified data in it. If error, data is deallocated.
	
	Input: Data and length.
	Output: Element or NULL
==========================================================================*/
pELEMENTTYPE XML_CreateOpaque (BYTE** ppbData, UINT32 iLength);


/*========================================================================
	XML_DeleteOpaque
==========================================================================
	The function deletes an element of type Type_Opaque and all data in it.

	The in-parameter ppEntity is set to point to NULL after the element 
	has been deleted. If the element is not an opaque element, it is left 
	unchanged.
  
	Input: ELEMENTTYPE** of type Type_Opaque
	Output: -
==========================================================================*/
void XML_DeleteOpaque (pELEMENTTYPE* ppOpaque);


/*========================================================================
	XML_GetNextElement
==========================================================================
	This function returns the next element in the list on the same level 
	as the input element.
	
	Input: Element
	Output: Element, NULL if no more elements
==========================================================================*/
pELEMENTTYPE XML_GetNextElement (pELEMENTTYPE pElement);


/*========================================================================
	XML_DeleteElement
==========================================================================
	This function deletes an element and deallocates all memory. The 
	function uses the function pointer DTD_DeleteElement. 

	The in-parameter ppElement is set to point to NULL after the element
	has been deleted.

	Input: ELEMENTTYPE**, and pWBXMLDECODESTR.
	Output: -
==========================================================================*/
void XML_DeleteElement (pELEMENTTYPE* ppElement, pWBXMLDECODESTR pDecStr);


/*========================================================================
	XML_DeleteElementList
==========================================================================
	This function deletes a list of elements (and deallocates all memory) 
	starting with the specified element. The function uses the function 
	XML_DeleteElement.

	The in-parameter ppElement is set to point to NULL after the element
	list has been deleted.

	Input: ELEMENTTYPE**, and pWBXMLDECODESTR.
	Output: -
==========================================================================*/
void XML_DeleteElementList (pELEMENTTYPE* ppElement, pWBXMLDECODESTR pDecStr);


/*========================================================================
	XML_AddElementToHead
==========================================================================
	This function stores the specified element at the start of the element 
	list. 

	Input: Element List and element
	Output: -
==========================================================================*/
void XML_AddElementToHead (pELEMENTTYPE* ppElementList, pELEMENTTYPE pElement);


/*========================================================================
	XML_AddElementToEnd
==========================================================================
	This function stores the specified element at the end of the element 
	list. 

	Input: Element List and element
	Output: -
==========================================================================*/
void XML_AddElementToEnd (pELEMENTTYPE* ppElementList, pELEMENTTYPE pElement);


#endif
