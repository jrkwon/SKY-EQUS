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
#ifndef _P_CoDec_H
#define _P_CoDec_H
/*========================================================================

	WAP Implementation Project

==========================================================================

	File: P_CoDec.h

	Description:

	Author: Jens Pommer, AU-System Radio AB

	Revision history:
  	Date    Sign	Comment
  	991115	JPR		First version
	000207	JPR		Updated to support new WBXML-decoder
	010126	NKE		New definition of Co_AppSpecParseMod

=========================================================================*/

/* Private include statements */

#include "cmmnrsrc.h"
#include "wbxmldec.h"
#include "hdrutil.h"


/*========================================================================

	Implemented function pointers for CO

=========================================================================*/

/*========================================================================
	Co_AppSpecParseMod
==========================================================================*/
void Co_AppSpecParseMod (pDECSTR pDecStr);


/*========================================================================
	Co_StoreAttributeValue
==========================================================================*/
BOOL Co_StoreAttributeValue (pDECSTR pDecStr, pELEMENTTYPE pElement,
							 UINT16 iAttribute, pELEMENTTYPE* ppAttrVal);


/*========================================================================
	Co_GetAttributeValue
==========================================================================*/
WCHAR* Co_GetAttributeValue (UINT16 iToken);


/*========================================================================
	Co_LiteralAttributeToToken
==========================================================================*/
UINT16 Co_LiteralAttributeToToken (WCHAR* pwchToken);


/*========================================================================
	Co_LiteralTagToToken
==========================================================================*/
UINT16 Co_LiteralTagToToken (WCHAR* pwchToken);


/*========================================================================
	Co_CheckPublicID
==========================================================================*/
BOOL Co_CheckPublicID (pDECSTR pDecStr, WCHAR* pwchPId, UINT8* piPId);

#endif
