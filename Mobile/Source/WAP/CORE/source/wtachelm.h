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

	WAP WTA Implementation Project

==========================================================================

	File: WTAChElm.h

	Description:

	Author: Jens Pommer, AU-System AB

	Revision history:
  	Date    Sign	Comment
  	990811  JPR		First version
	000207	JPR		Updated to support new WBXML-decoder
	000630	JPR		Updated for WAP 1.2.1
	001011	ASPN	Lower-case filenames

==========================================================================*/

#ifndef _WTACHELMHANDLERH
#define _WTACHELMHANDLERH

#include "wbxmlelm.h"

/*========================================================================

	Implemented function pointers for Channel elements

=========================================================================*/

/*========================================================================
	Ch_CreateElement
==========================================================================
	Channel specific implemetation.

	Creates an element of the specified type and sets the default values.
	The returned element is of the type tELEMENTTYPE, but can be casted
	to the correct type (stored in the element, consult the definitions in
	the file WBXMLDef.h) if the attributes are required.

	It is the caller's responsibility to deallocate the memory (with a 
	call to the function XML_DeleteElement).
	
	Input: pWBXMLDECODESTR (not NULL), Type of element to be created
	Output: Pointer to a struct of the type tELEMENTTYPE or NULL.
==========================================================================*/
pELEMENTTYPE Ch_CreateElement (pDECSTR pDecStr, UINT16 iType);


/*========================================================================
	Ch_DeleteElement
==========================================================================
	Channel specific implemetation.

	A function of this type is used for deleting an element and all its
	content.

	Deletes an element and all its content and frees the memory. The in-
	parameter ppElement is set to point to NULL after the element has been 
	deleted.

	Input: pWBXMLDECODESTR (not NULL), ELEMENTTYPE**
	Output: -
==========================================================================*/
void Ch_DeleteElement (pDECSTR pDecStr, pELEMENTTYPE* ppElement);


/*========================================================================
	Ch_AddContent
==========================================================================
	Channel specific implemetation.

	The function returns TRUE if the content is legal in the element and
	stored, FALSE otherwise. The function SHOULD consider the Version 
	attributein the Decode Struct when the decision is made. If the 
	function returns FALSE, the calling function will set the Illegal 
	Content Warning in the Decode Struct. 
	
	If the content is stored, the in-parameter ppContent MUST be set to 
	point to NULL. Otherwise it MUST be unchanged.

	Input: pWBXMLDECODESTR (not NULL), Element and content to be stored.
	Output: TRUE/FALSE
==========================================================================*/
BOOL Ch_AddContent (pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppContent);


/*========================================================================
	Ch_GetContent
==========================================================================
	Channel specific implemetation.

	A function of this type is used to retrieve the first content from an
	element.

	If no content is present or if the pElement is NULL, NULL is returned.
	
	NOTE! The returned data is not a copy and therefore MUST NOT be 
	deallocated. The data is deallocated when the structure is deleted.

	Input: Element
	Output: First Content in the Element or NULL
==========================================================================*/
pELEMENTTYPE Ch_GetContent (pELEMENTTYPE pElement);


/*========================================================================

	Attribute handling functions for the channel elements.

==========================================================================*/

/*========================================================================
	Ch_Set(attribute name)
==========================================================================
	The function accepts a WCHAR** with a number and converts it to an
	INT32. The text is also deallocated. The number is defined as

	[-] (0..9)*

	The result is stored in the piResult parameter if the conversion is
	successful.
   
	Input: WCHAR **, INT32*
	Output: BOOL
==========================================================================*/
BOOL Ch_ConvertNumber32 (WCHAR** ppwchNumber, INT32* piResult);


/*========================================================================
	Setting channel attributes (attribute name)
==========================================================================
	The following functions sets the attribute values in the elements. If
	the attribute field and the attribute value is valid for the element 
	it is and stored and TRUE is returned. Otherwise, FALSE is returned. 

	NOTE! It is the callers responsibility to deallocate all parts NOT 
	used by the functions. For example, if ppAlign is not used in the 
	function Ch_EventId, this memory must be deallocated.
	
	Input: Element (NOT NULL) and attribute value.						
	Output: BOOL
==========================================================================*/
BOOL Ch_SetMaxspace				(pDECSTR pDecStr, pELEMENTTYPE pElementObj, pELEMENTTYPE *ppMaxspace);
BOOL Ch_SetBase						(pDECSTR pDecStr, pELEMENTTYPE pElementObj, pELEMENTTYPE *ppBase);
BOOL Ch_SetEventId				(pDECSTR pDecStr, pELEMENTTYPE pElementObj, pELEMENTTYPE *ppEventId);
BOOL Ch_SetChannelID			(pDECSTR pDecStr, pELEMENTTYPE pElementObj, pELEMENTTYPE *ppChannelId);
BOOL Ch_SetHref						(pDECSTR pDecStr, pELEMENTTYPE pElementObj, pELEMENTTYPE *ppHref);
BOOL Ch_SetLastmod				(pDECSTR pDecStr, pELEMENTTYPE pElementObj, pELEMENTTYPE *ppLastmod);
BOOL Ch_SetEtag						(pDECSTR pDecStr, pELEMENTTYPE pElementObj, pELEMENTTYPE *ppEtag);
BOOL Ch_SetMd5						(pDECSTR pDecStr, pELEMENTTYPE pElementObj, pELEMENTTYPE *ppMd5);
BOOL Ch_SetSuccess				(pDECSTR pDecStr, pELEMENTTYPE pElementObj, pELEMENTTYPE *ppSuccess);
BOOL Ch_SetFailure				(pDECSTR pDecStr, pELEMENTTYPE pElementObj, pELEMENTTYPE *ppFailure);
BOOL Ch_SetUserAccessible	(pDECSTR pDecStr, pELEMENTTYPE pElementObj, pELEMENTTYPE *ppUserAcc);

#endif
