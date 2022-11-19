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

	File: WTAChDec.h

	Description:

	Author: Jens Pommer, AU-System Radio AB

	Revision history:

  	Date    Sign	Comment
  	990811  JPR		First version
		000204	JPR		Updated to support new WBXML-decoder
		000630	JPR		Corrections
		001011	ASPN	Lower-case filenames

==========================================================================*/
#ifndef _WTACHDEC_H
#define _WTACHDEC_H

#include "wtachelm.h"
#include "wbxmldec.h"
#include "wmlparse.h"


/*========================================================================

	Implemented function pointers for channels

=========================================================================*/

/*========================================================================
	Ch_AppSpecParseMod
==========================================================================*/
void Ch_AppSpecParseMod(pDECSTR pDecStr, pELEMENTTYPE *ppElement, pELEMENTTYPE pParent);


/*========================================================================
	Ch_StoreAttributeValue
==========================================================================*/
BOOL Ch_StoreAttributeValue (pDECSTR pDecStr, pELEMENTTYPE pElement, 
							 UINT16 iAttribute, pELEMENTTYPE* ppAttrVal);


/*========================================================================
	Ch_GetAttributeValue
==========================================================================*/
WCHAR* Ch_GetAttributeValue (UINT16 iToken);


/*========================================================================
	Ch_LiteralAttributeToToken
==========================================================================*/
UINT16 Ch_LiteralAttributeToToken (WCHAR* pwchToken);


/*========================================================================
	Ch_LiteralTagToToken
==========================================================================*/
UINT16 Ch_LiteralTagToToken (WCHAR* pwchToken);


/*========================================================================
	Ch_CheckPublicID
==========================================================================*/
BOOL Ch_CheckPublicID (pDECSTR pDecStr, WCHAR* pwchPId, UINT8* piPId);


#endif
