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
    FILE: wbxmldec.h
  
    General WBXML-decoder.

    Rev history:
    990302 JPR   Created
    990317 JPR   Modified
    990421 JPR   Updated

    991124 JPR   Redesign of Element Handling, Text Handling,
                 and Error Handling (Consult separate document).
                 Updates for WAP 1.2 are also included.
    991125 JPR   Application specific parts removed
    991207 JPR   Minor changes
    000303 JPR   Function WBXML_GetContentType added
    000526 JPR   Function WBXML_GetContentType modified
    000814 JPR   Functions WBXML_StepPastAllPIs and 
                 WBXML_StepPastOnePI added
    010105 NKE   WBXML decoder rewritten to avoid recursion and
                 enable scheduled execution. Replaced WBXML_DecodeElement
                 WBXML_DecodeContent with WBXML_Decode and WBXML_InitDedcode.
                 Added helpfunctions WBXML_DeleteCurrentElement and
                 WBXML_AddElement.
    010126 NKE   Updated the definition of WBXML_InitDecode.
==========================================================================*/

#ifndef CWAPWBXMLDEC_H
#define CWAPWBXMLDEC_H

#include "wbxmldef.h"


/*========================================================================
	WBXML_DelStrTab
==========================================================================
	The function deletes the string table in the decode struct. 

	NOTE! If the string table is deleted, the structure is useless!

	Input: Pointer to WBXMLDECODESTR
	Output: -
==========================================================================*/
void WBXML_DelStrTab (pWBXMLDECODESTR pDecStr);


/*========================================================================
	WBXML_GetStrTabVal
==========================================================================
	The function returns a pointer to the string at the specified location 
	in the string table. 

	NOTE! The string MUST NOT be deleted by the caller. When the WBXML-
	decoding is complete, the string table and the strings are removed.

	Input: Pointer to WBXMLDECODESTR and Location in string table (UINT32)
	Output: Pointer to WCHAR, NULL if not found.
==========================================================================*/
WCHAR* WBXML_GetStrTabVal (pWBXMLDECODESTR pDecStr, UINT32 iLoc);


/*========================================================================
	WBXML_CreateWBXMLDecStr
==========================================================================
	The function creates a WBXML decode structure and sets the default 
	values.

	NOTE! The structure should be deleted with a call to the function 
	WBXML_DeleteWBXMLDecStr.

	Input: -
	Output: Pointer to WBXMLDECODESTR, NULL if error. 
==========================================================================*/
pWBXMLDECODESTR WBXML_CreateWBXMLDecStr (void);


/*========================================================================
	WBXML_DeleteWBXMLDecStr
==========================================================================
	The function deletes a WBXML decode structure and all its attributes
	(including the string table).

	Input: ** to WBXMLDECODESTR
	Output: -
==========================================================================*/
void WBXML_DeleteWBXMLDecStr (pWBXMLDECODESTR *ppDecStr);


/*========================================================================
	WBXML_CopyCharToWChar
==========================================================================
	The function copies a byte string and stores the result in a WCHAR 
	string. Sets the Abort flag if not successful. (out of memory - fatal 
	error.)


	Input: pWBXMLDECODESTR (not NULL) and pointer to CHAR (not NULL)
	Output: WCHAR *, NULL if error.
==========================================================================*/
WCHAR* WBXML_CopyCharToWChar (pWBXMLDECODESTR pDecStr, CHAR* src);


/*========================================================================
	WBXML_CheckIfGlobalText
==========================================================================
	The function checks if the specified byte is a text. 

	Text is defined as:
	( string | extension | entity )

	Input: bToken (BYTE)
	Output: TRUE if Text, FALSE otherwise
==========================================================================*/
BOOL WBXML_CheckIfGlobalText (BYTE bToken);


/*========================================================================
	WBXML_GetSTR_I
==========================================================================
	The function reads an inline string in the from the byte-stream and 
	returns a copy of it. The charset attribute in the WBXMLDECODESTR 
	struct is used to determine which decoding function to use. The 
	pointer to the data in the decode struct is updated to point at the 
	first byte after the inline string.

	Input: pWBXMLDECODESTR (not NULL)
	Output: WCHAR *, NULL if error.
==========================================================================*/
WCHAR* WBXML_GetSTR_I (pWBXMLDECODESTR pDecStr);


/*========================================================================
	WBXML_GetMBUINT32
==========================================================================
	The function reads a mbuint32 from the byte-stream and returns it. The 
	pointer to the data in the decode struct is updated to point at the 
	first byte after the mbuint32.

	Input: pWBXMLDECODESTR (not NULL)
	Output: UINT32
==========================================================================*/
UINT32 WBXML_GetMBUINT32 (pWBXMLDECODESTR pDecStr);


/*========================================================================
	WBXML_StepPastOnePI
==========================================================================
	The function steps past an PI element

	Input: pWBXMLDECODESTR (not NULL)
	Output: -
==========================================================================*/
void WBXML_StepPastOnePI (pWBXMLDECODESTR pDecStr);


/*========================================================================
	WBXML_StepPastAllPIs
==========================================================================
	The function steps past all concecutive PI elements

	Input: pWBXMLDECODESTR (not NULL)
	Output: -
==========================================================================*/
void WBXML_StepPastAllPIs (pWBXMLDECODESTR pDecStr);


/*========================================================================
	WBXML_DecodeGlobalText
==========================================================================
	The function decodes one string, extension or entity and returns the
	result as an element. If none is found, NULL is returned. The pointer 
	to the data in the decode struct is updated to point at the first byte 
	after the text.

	Input: pWBXMLDECODESTR (not NULL)
	Output: pELEMENTTYPE
==========================================================================*/
pELEMENTTYPE WBXML_DecodeGlobalText (pWBXMLDECODESTR pDecStr);


/*========================================================================
	WBXML_DecodeAttributeValue
==========================================================================
	Returns one attribute value as a sequence of elements. NULL if no 
	attrValues. (pDecStr->fAbort is set if fatal error). The pointer to
	the data in the decode struct is updated to point at the first byte 
	after the attribute value.

	Input: pWBXMLDECODESTR (not NULL)
	Output: List of elements
==========================================================================*/
pELEMENTTYPE WBXML_DecodeAttributeValue (pWBXMLDECODESTR pDecStr);


/*========================================================================
	WBXML_DecodeAttributes
==========================================================================
	Decodes all attributes for an element and stores them. The pointer to
	the data in the decode struct is updated to point at the first byte 
	after the attributes.

	Input: pWBXMLDECODESTR (not NULL) and pointer to ELEMENTTYPE
	Output: -
==========================================================================*/
void WBXML_DecodeAttributes (pWBXMLDECODESTR pDecStr, pELEMENTTYPE pElement);


/*========================================================================
	WBXML_DeleteCurrentElement
==========================================================================
	Delete element internally marked as current.
==========================================================================*/
void WBXML_DeleteCurrentElement(pWBXMLDECODESTR decodeStruct);


/*========================================================================
	WBXML_AddElement
==========================================================================
	Add an element 'newElement' to the decode data structure handled by
	'decodeStructure'.
==========================================================================*/
void WBXML_AddElement(pWBXMLDECODESTR decodeStruct, pELEMENTTYPE newElement);


/*========================================================================
	WBXML_InitDecode
==========================================================================
	Init the decoding. Called prior to WBXML_Decode.
	
	'decodeStruct' is the structure connected to the current document.
	Several documents can be decoded simultanous, each connected to its
	own decode structure. 'parent' is the parent element, if it exist,
	otherwise NULL: 
	
	If 'scheduled' is set to TRUE, then WBXML_Decode will return  after
	decoding a few elements irrespectiv of more data is to decode or not.
	If 'scheduled' is set to FALSE, then the entire document is decoded
	without interruptions.
==========================================================================*/
void WBXML_InitDecode (pWBXMLDECODESTR decodeStruct, BOOL scheduled);


/*========================================================================
	WBXML_Decode
==========================================================================
	The function decodes one element from the instream. The function
	creates the appropriate element and decodes the attributes and the 
	content recursively. The pointer to the data in the decode struct is 
	updated to point at the first byte after the element.

	This function must be called repeatedly until *finished is set to
	TRUE. Use WBXML_InitDecode before calling for the first time.

	Element:
	( [switchPage] stag ) [ 1*attribute END ] [ *content END ]

	Content:
	( element | string | extension | entity | pi | opaque )

	Input: pWBXMLDECODESTR (not NULL)
	Output: Pointer to an element or NULL and a flag finished indicating
	        if ready.
==========================================================================*/
pELEMENTTYPE WBXML_Decode (pWBXMLDECODESTR decodeStruct, BOOL *finished);


/*========================================================================
	WBXML_DecodeProlog
==========================================================================
	The function decodes the WBXML prolog and stores the found data in the
	Decode struct. If the operation is successful, TRUE is returned, 
	otherwise FALSE is returned.

	Prolog:
	( version publicid charset strtbl )

	Input: pWBXMLDECODESTR (not NULL)
	Output: TRUE if OK, FALSE otherwise.
==========================================================================*/
BOOL WBXML_DecodeProlog (pWBXMLDECODESTR pDecStr);


/*========================================================================
	WBXML_GetContentType
==========================================================================
	Examines the PublicID of the content and sets the pbContentType with
	the correct	WSP-content type. If the content type cannot be decided,
	or it is 'unknown' WML is assumed. If one of the unsupported content 
	types is found (see below), pbContentType is set to unknown.

	TRUE is returned if the content is well known unless it is one of the 
	following (not supported):

		WTA Event
		WML 1.0	

	Otherwise FALSE is returned.

	Prolog:
	( version publicid charset strtbl )

	Input: Data, Pointer to first byte AFTER data, pointer to BYTE
	Output: TRUE if OK, FALSE otherwise.
==========================================================================*/
BOOL WBXML_GetContentType (BYTE* pbData, BYTE* pbEnd, BYTE* pbContentType);


#endif
