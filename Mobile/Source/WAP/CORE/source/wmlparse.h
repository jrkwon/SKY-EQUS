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

		991126	JPR		Updated to support new generic WBXML-decoder.
						Updates for WAP 1.2 are also included.
		991207	JPR		Minor corrections
		
==========================================================================*/

#ifndef _NWMLPARSERH
#define _NWMLPARSERH

#include "wmlelm.h"
#include "wbxmldef.h"

/*========================================================================

	Implemented function pointers for WML

=========================================================================*/

/*========================================================================
	WML_ModifyParse
==========================================================================
	WML-specific implementation of DTD_AppSpecParseModFunc.
  
	Modifies the structure for ONEVENT, OPTION, SELECT and for TEMPLATE 
	and CARD.
==========================================================================*/
void WML_ModifyParse (pDECSTR pDecStr, pELEMENTTYPE *ppElement, 
					  pELEMENTTYPE  pParent);


/*========================================================================
	WML_StoreAttributeValue
==========================================================================
	WML-specific implementation of DTD_StoreAttributeValueFunc.
==========================================================================*/
BOOL WML_StoreAttributeValue (pDECSTR pDecStr, pELEMENTTYPE pElement,
							  UINT16 iAttribute, pELEMENTTYPE* ppAttrVal);


/*========================================================================
	WML_GetAttributeValue
==========================================================================
	WML-specific implementation of DTD_GetAttributeValueFunc.
==========================================================================*/
WCHAR* WML_GetAttributeValue (UINT16 iToken);


/*========================================================================
	WML_LiteralAttributeToToken
==========================================================================
	WML-specific implementation of DTD_LiteralAttributeToTokenFunc
==========================================================================*/
UINT16 WML_LiteralAttributeToToken (WCHAR* pwchToken);


/*========================================================================
	WML_LiteralTagToToken
==========================================================================
	WML-specific implementation of DTD_LiteralTagToTokenFunc
==========================================================================*/
UINT16 WML_LiteralTagToToken (WCHAR* pwchToken);


/*========================================================================
	WML_CheckPublicID
==========================================================================
	WML-specific implementation of DTD_CheckPublicIDFunc.
==========================================================================*/
BOOL WML_CheckPublicID (pDECSTR pDecStr, WCHAR* pwchPId, UINT8* piPId);



/*========================================================================

	Other functions

=========================================================================*/

/*========================================================================
	WML_CheckIfOnlyWhiteSpace
==========================================================================
	Returns TRUE if the text contains only white space characters or if it 
	is NULL. FALSE otherwise.  

	Input: WCHAR*
	Output: TRUE if text NULL or only white space, FALSE otherwise
==========================================================================*/
BOOL WML_CheckIfOnlyWhiteSpace (WCHAR* pwchText);



#endif
