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

	WAP PUSH Implementation Project

==========================================================================

	File: pushelm.h

	Description:

	Functions for handling the different elements in the PUSH speci-
	fications, i.e., Service Indication (SI), Service Loading (SL), and
	Cache Operation (CO)

	Author: Jens Pommer, AU-System Radio AB

	Revision history:
  	Date      Sign	Comment
  	991112    JPR   First version
	991116	  JPR	Attribute functions added

	000131	  JPR	Changes to support generic WBXML-decoder
	000208	  JPR	Function Si_DateTime2Int added
	010119	  NKE	Added Si_ValidContent, Sl_ValidContent and Co_ValidContent
					for the new decoder in wbxmldec.c. Removed corresponding
					AddContent.

==========================================================================*/

#ifndef _PUSHELMHANDLERH
#define _PUSHELMHANDLERH

#include "wbxmlelm.h"

/*========================================================================

	Functions for handling the Service Indication (SI), Service Loading 
	(SL), and Cache Operation (CO) elements.

=========================================================================*/

/*========================================================================
	Si_CreateElement
	Sl_CreateElement
	Co_CreateElement
==========================================================================
	PUSH-specific implemetation.

	Creates an element of the specified type and sets the default values.
	The returned element is of the type tELEMENTTYPE, but can be casted
	to the correct type (stored in the element, consult the definitions in
	the file WBXMLDef.h) if the attributes are required.

	It is the caller's responsibility to deallocate the memory (with a 
	call to the function XML_DeleteElement).
	
	Input: pWBXMLDECODESTR (not NULL), Type of element to be created
	Output: Pointer to a struct of the type tELEMENTTYPE or NULL.
==========================================================================*/
pELEMENTTYPE Si_CreateElement (pDECSTR pDecStr, UINT16 iType);
pELEMENTTYPE Sl_CreateElement (pDECSTR pDecStr, UINT16 iType);
pELEMENTTYPE Co_CreateElement (pDECSTR pDecStr, UINT16 iType);


/*========================================================================
	Si_DeleteElement
	Sl_DeleteElement
	Co_DeleteElement
==========================================================================
	PUSH-specific implemetation.

	A function of this type is used for deleting an element and all its
	content.

	Deletes an element and all its content and frees the memory. The in-
	parameter ppElement is set to point to NULL after the element has been 
	deleted.

	Input: pWBXMLDECODESTR (not NULL), ELEMENTTYPE**
	Output: -
==========================================================================*/
void Si_DeleteElement (pDECSTR pDecStr, pELEMENTTYPE* ppElement);
void Sl_DeleteElement (pDECSTR pDecStr, pELEMENTTYPE* ppElement);
void Co_DeleteElement (pDECSTR pDecStr, pELEMENTTYPE* ppElement);


/*========================================================================
	Si_ValidContent
	Sl_ValidContent
	Co_ValidContent
==========================================================================
	PUSH-specific implemetation.

	The function returns TRUE if the content is legal in the element,
	FALSE otherwise. The function SHOULD consider the Version 
	attributein the Decode Struct when the decision is made. If the 
	function returns FALSE, the calling function will set the Illegal 
	Content Warning in the Decode Struct. 
	
	Input: pWBXMLDECODESTR (not NULL) and element.
	Output: TRUE/FALSE
==========================================================================*/
BOOL Si_ValidContent (pDECSTR pDecStr, pELEMENTTYPE pElement);
BOOL Sl_ValidContent (pDECSTR pDecStr, pELEMENTTYPE pElement);
BOOL Co_ValidContent (pDECSTR pDecStr, pELEMENTTYPE pElement);


/*========================================================================
	Si_GetContent
	Sl_GetContent
	Co_GetContent
==========================================================================
	PUSH-specific implemetation.

	A function of this type is used to retrieve the first content from an
	element.

	If no content is present or if the pElement is NULL, NULL is returned.
	
	NOTE! The returned data is not a copy and therefore MUST NOT be 
	deallocated. The data is deallocated when the structure is deleted.

	Input: Element
	Output: First Content in the Element or NULL
==========================================================================*/
pELEMENTTYPE Si_GetContent (pELEMENTTYPE pElement);
pELEMENTTYPE Sl_GetContent (pELEMENTTYPE pElement);
pELEMENTTYPE Co_GetContent (pELEMENTTYPE pElement);


/*========================================================================

	Attribute handling functions for the PUSH elements.

==========================================================================*/

/*========================================================================
	Si_DateTime2Int
==========================================================================
Purpose: Convert from Date Time format to an UINT32 in seconds from 1970.
Params:  pbDateTime, iDateTimeLen
Return:  UINT32 in seconds counting from 1970-01-01.
=========================================================================*/
UINT32 Si_DateTime2Int (BYTE* pbDateTime,UINT32 iDateTimeLen);


/*========================================================================
	Setting push attributes (attribute name)
==========================================================================
	The following functions sets the attribute values in the elements. If
	the attribute field and the attribute value is valid for the element 
	it is and stored and TRUE is returned. Otherwise, FALSE is returned. 

	NOTE! It is the callers responsibility to deallocate all parts NOT 
	used by the functions. For example, if ppAlign is not used in the 
	function WML_SetAlign, this memory must be deallocated.
	
	Input: Element (NOT NULL) and attribute value.						
	Output: BOOL
==========================================================================*/

/* Si */
BOOL Si_SetAction	(pDECSTR pDecStr, pELEMENTTYPE pElementObj, pELEMENTTYPE* ppAction, UINT8 iAction);
BOOL Si_SetCreated	(pDECSTR pDecStr, pELEMENTTYPE pElementObj, pELEMENTTYPE* ppCreated);
BOOL Si_SetHref		(pDECSTR pDecStr, pELEMENTTYPE pElementObj, pELEMENTTYPE* ppHref);
BOOL Si_SetExpires	(pDECSTR pDecStr, pELEMENTTYPE pElementObj, pELEMENTTYPE* ppExpires);
BOOL Si_SetSiId		(pDECSTR pDecStr, pELEMENTTYPE pElementObj, pELEMENTTYPE* ppSiId);
BOOL Si_SetClass	(pDECSTR pDecStr, pELEMENTTYPE pElementObj, pELEMENTTYPE* ppClass);

/* Sl */
BOOL Sl_SetAction	(pDECSTR pDecStr, pELEMENTTYPE pElementObj, pELEMENTTYPE* ppAction, UINT8 iAction);
BOOL Sl_SetHref		(pDECSTR pDecStr, pELEMENTTYPE pElementObj, pELEMENTTYPE* ppHref);

/* Co */
BOOL Co_SetURI		(pDECSTR pDecStr, pELEMENTTYPE pElementObj, pELEMENTTYPE* ppURI);

#endif
