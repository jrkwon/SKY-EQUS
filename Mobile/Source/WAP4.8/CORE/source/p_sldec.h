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
#ifndef _P_SlDec_H
#define _P_SlDec_H
/*========================================================================

	WAP Implementation Project

==========================================================================

	File: P_SlDec.h

	Description:

	Author: Jens Pommer, AU-System Radio AB

	Revision history:
  	Date    Sign	Comment
  	991115	JPR		First version
	000207	JPR		Updated to support new WBXML-decoder
	010126	NKE		New definition of Sl_AppSpecParseMod

=========================================================================*/

/* Private include statements */

#include "cmmnrsrc.h"
#include "wbxmldef.h"
#include "hdrutil.h"
#include "wmlparse.h"
#include "wbxmlelm.h"
#include "pushdef.h"
#include "pushelm.h"


/*========================================================================

	Implemented function pointers for SL

=========================================================================*/

/*========================================================================
	Sl_AppSpecParseMod
==========================================================================*/
void Sl_AppSpecParseMod (pDECSTR pDecStr);


/*========================================================================
	Sl_StoreAttributeValue
==========================================================================*/
BOOL Sl_StoreAttributeValue (pDECSTR pDecStr, pELEMENTTYPE pElement,
							 UINT16 iAttribute, pELEMENTTYPE* ppAttrVal);


/*========================================================================
	Sl_GetAttributeValue
==========================================================================*/
WCHAR* Sl_GetAttributeValue (UINT16 iToken);


/*========================================================================
	Sl_LiteralAttributeToToken
==========================================================================*/
UINT16 Sl_LiteralAttributeToToken (WCHAR* pwchToken);


/*========================================================================
	Sl_LiteralTagToToken
==========================================================================*/
UINT16 Sl_LiteralTagToToken (WCHAR* pwchToken);


/*========================================================================
	Sl_CheckPublicID
==========================================================================*/
BOOL Sl_CheckPublicID (pDECSTR pDecStr, WCHAR* pwchPId, UINT8* piPId);

#endif
