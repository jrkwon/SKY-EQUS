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
	FILE: WMLParse.h
  
	Interface file for WML-specific parse

	Rev history:
		980706	JPR		Created
		981223	JPR		Event concept changed	
		990125	JPR		EndParser updated					
		990211	JPR		Corrections						
		990212	JPR		Attribute handling corrected		
		990312	JPR		Major changes. Handles the WML-specific parts 
						of the parsing.
		990329	JPR		Corrections
		990422	JPR		Support for LITERALS
		990512	JPR		Extendend error handling added
		990528	JPR		Function WML_CheckIfOnlyWhiteSpace added
   ================================================================== */

#ifndef _NWMLPARSERH
#define _NWMLPARSERH

#include "WMLElm.h"
#include "WBXMLDec.h"

/* -----------------------------------------------
	Examines the PublicID and verifies it.
	Input: UINT8 / WCHAR *
	Output: TRUE if valid ID, FALSE otherwise.
   ----------------------------------------------- */
BOOL WML_CheckPublicIDInt (pWBXMLDECODESTR pDecStr, UINT8 iPId);

BOOL WML_CheckPublicIDWChar (pWBXMLDECODESTR pDecStr, WCHAR *pchId);

/* -----------------------------------------------
	Sets the Charset according to the WML 1.1-spec
	section 6.1 (page 11)
	Input: pWBXMLDECODESTR and INT16
	Output: -
   ----------------------------------------------- */
void WML_SetParserCharset (pWBXMLDECODESTR pDecStr, INT16 iCharset);

/* -----------------------------------------------
	Returns one WML attribute value. The abort 
	is set if fatal error.
    Input: pWBXMLDECODESTR
	Output: WCHAR *, NULL if no value.
   ----------------------------------------------- */
WCHAR *WML_ParseAttributeValue (pWBXMLDECODESTR pDecStr);

/* -----------------------------------------------
	The function parses one WML-specific token of 
	the instream and stores the data in the element.
	If the attribute is not legal in that element it
	is ignored. The Data pointer should point to 
	the attribute value. (Uses only code page 0)

	Input: pWBXMLDECODESTR, pELEMENTTYPE and BYTE
	Output: -
   ----------------------------------------------- */
void WML_ParseAttribute (pWBXMLDECODESTR pDecStr, pELEMENTTYPE pElement, 
						 BYTE bAttribute); 

/* -----------------------------------------------
	Uses binary search to find a matching string
	in the table (TokenTab) and returns the byte 
	at the same index in the ByteTab. If a matching
	string is not found, Type_Literal is returned.
	
	NOTE! The TokenTab MUST be sorted!

	Input: pchToken (Not NULL), TokenTab and Byte-
		   Tab with iTabLen number of enties, none
		   of them NULL.
	Output: Correct BYTE, Type_Literal if not found
   ----------------------------------------------- */
BYTE WML_ConvertTokenToByte (WCHAR *pchToken, const WCHAR * const ppchTokenTab[],
							 const BYTE *pbByteTab, UINT16 iTabLen);

/* ------------------------------------------------
	Converts the LITERAL attribute name to the 
	corresponding byte. If the tag is unknown 
	Type_Literal is returned.	
	
	Input: Pointer to WBXMLDECODESTR and position 
		   in string table.
	Output: BYTE
   ------------------------------------------------*/
BYTE WML_ConvertAttributeToByte (pWBXMLDECODESTR pDecStr, UINT32 iPos);

/* ------------------------------------------------
	Converts the LITERAL tag to the corresponding
	byte. If the tag is unknown Type_Literal is 
	returned.	
	
	Input: Pointer to WBXMLDECODESTR and position 
		   in string table.
	Output: BYTE
   ------------------------------------------------*/
BYTE WML_ConvertTagToByte (pWBXMLDECODESTR pDecStr, UINT32 iPos);

/* ------------------------------------------------
	Returns TRUE if the text contains only white 
	space characters or if it is NULL. FALSE 
	otherwise.  

	Input: WCHAR *
	Output: TRUE if text NULL or only white space,
			FALSE otherwise
   ------------------------------------------------*/
BOOL WML_CheckIfOnlyWhiteSpace (WCHAR *pchText);

/* ---------------------------------------------
	Modifies the structure for ONEVENT, OPTION,
	SELECT and for TEMPLATE and CARD.
	Input: Pointer WBXMLDECODESTR, ** to element
		   and pointer to parent element.
	Output: -
   --------------------------------------------- */
void WML_ModifyParse (pWBXMLDECODESTR pDecStr, pELEMENTTYPE *ppElement, pELEMENTTYPE pParent);

#endif
