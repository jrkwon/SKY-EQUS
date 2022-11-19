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
	FILE: WBXMLDec.h
  
	General WBXML-decoder.

	Rev history:
		990302	JPR		Created
		990317	JPR		Modified
		990421	JPR		Updated
   ================================================================== */

#ifndef CWAPWBXMLDEC_H
#define CWAPWBXMLDEC_H

#include "WBXMLDef.h"

/* -----------------------------------------------
	Returns a pointer to the string at the specified 
	location in the string table. When the parsing 
	is complete, the string table and the strings are 
	removed.
	Input: Pointer to WBXMLDECODESTR and Location 
		  (UINT32)
	Output: Pointer to WCHAR (NOT COPIED), NULL if 
			error.
   ----------------------------------------------- */
WCHAR *WBXML_GetStrTabVal (pWBXMLDECODESTR pDecStr, UINT32 iLoc) ;

/* ---------------------------------------------
	Creates a WBXML decode structure and sets
	the default values.
	Input: -
	Output: Pointer to WBXMLDECODESTR, NULL if
			error. 
   --------------------------------------------- */
pWBXMLDECODESTR WBXML_CreateWBXMLDecStr (void);

/* ---------------------------------------------
	Deletes a WBXML decode structure and all its
	values (including the string table).
	Input: ** to WBXMLDECODESTR
	Output: -
   --------------------------------------------- */
void WBXML_DeleteWBXMLDecStr (pWBXMLDECODESTR *ppDecStr);

/* ---------------------------------------------
	Increases the pbCurByte in the 
	pWBXMLDECODESTR.
	Input: Pointer to WBXMLDECODESTR
	Output: -
   --------------------------------------------- */
void WBXML_StepByte (pWBXMLDECODESTR pDecStr);

/* ---------------------------------------------
	Copies a byte string and stores the result 
	in a WCHAR string. Sets the Abort flag if not
	successful. (out of memory - fatal error.)
	Input: Pointer to WBXMLDECODESTR (not NULL)
		   and pointer to CHAR (not NULL)
	Output: WCHAR *, NULL if error.
   --------------------------------------------- */
WCHAR *WBXML_CopyCharToWChar (pWBXMLDECODESTR pDecStr, CHAR* src);

/* ---------------------------------------------
	Reads an inline string in the from the byte-
	stream and returns a copy of it. The charset
	attribute in the WBXMLDECODESTR struct is 
	used to determine the decoding function.
	Input: Pointer to WBXMLDECODESTR (not NULL)
	Output: WCHAR *, NULL if error.
   --------------------------------------------- */
WCHAR *WBXML_GetSTR_I (pWBXMLDECODESTR pDecStr);

/* ---------------------------------------------
	Checks if the current byte is a text.

	Text is :
	( string | extension | entity )

	Input: Pointer WBXMLDECODESTR.
	Output: TRUE if Text, FALSE otherwise
   --------------------------------------------- */
BOOL WBXML_CheckIfText (pWBXMLDECODESTR pDecStr);

/* ---------------------------------------------
	Returns TRUE if the current byte is an 
	Attribute Value token. That is, NOT Attribute 
	Start Token or Global token END or LITERAL or 
	end of instream. The global token END also 
	performs a StepByte.	
	Input: Pointer WBXMLDECODESTR.
	Output: TRUE if Attribute Value token, FALSE 
			otherwise.
   --------------------------------------------- */
BOOL WBXML_CheckIfAttributeValue (pWBXMLDECODESTR pDecStr);

/* ----------------------------------------------
	Parses all consecutive strings, extensions or 
	entities in the instream and returns it as an 
	ELEMENT of type text.

	Text is :
	( string | extension | entity )

	Input: Pointer WBXMLDECODESTR.
	Output: Pointer to TEXTELEMENT, NULL if Error
			or no text found.
   ---------------------------------------------- */
pTEXTELEMENT WBXML_ParseGlobalText (pWBXMLDECODESTR pDecStr);

/* ---------------------------------------------
	Returns one attribute value as a TEXTELEMENT. 
	NULL if no attrValues. 

	attrValue is:
	( ATTRVALUE | string | extension | entity ) 

	Input: Pointer WBXMLDECODESTR 
	Output: Pointer to TEXTELEMENT or NULL.
			(pDecStr->fAbort is set if fatal error)
   --------------------------------------------- */
pTEXTELEMENT WBXML_ParseAttributeValue (pWBXMLDECODESTR pDecStr);


/* ---------------------------------------------
	Parses the attributes for an element and 
	stores them.

	Input: Pointer WBXMLDECODESTR and pointer to
		   ELEMENTTYPE
	Output: -
   --------------------------------------------- */
void WBXML_ParseAttributes (pWBXMLDECODESTR pDecStr, pELEMENTTYPE pElement);


/* ---------------------------------------------
	Application specific modifications to the
	element. The DTD is checked and the appro-
	priate function is called.
	
	  NOTE! The Element may be removed!

	Input: Pointer WBXMLDECODESTR, ** to Element 
		   and pointer to parent element.
	Output: -
   --------------------------------------------- */
void XML_AppSpecParseMod (pWBXMLDECODESTR pDecStr, pELEMENTTYPE *ppElement, pELEMENTTYPE  pParent);

/* ------------------------------------------------
	Parses the instream for an element. Creates the 
	appropriate element	and parses the attributes 
	and the content.

	Element:
	( stag [ 1*attribute END ] [ *content END ] )

	Content:
	( element | string | extension | entity | pi 
	| opaque )

	Input: Pointer to WBXMLDECODESTR and parent 
		   element.
	Output: Pointer to an element or NULL
   ------------------------------------------------*/
pELEMENTTYPE WBXML_ParseElement (pWBXMLDECODESTR pDecStr, pELEMENTTYPE pParent);

/* -----------------------------------------------
    Parses the Prolog.

	Prolog:
	( version publicid charset strtbl )

	Input: Pointer to WBXMLDECODESTR.
	Output: TRUE if OK, FALSE otherwise.
   ----------------------------------------------- */
BOOL WBXML_ParseProlog (pWBXMLDECODESTR pDecStr);

#endif
