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
#ifndef _P_SiDec_H
#define _P_SiDec_H
/*========================================================================

	WAP Implementation Project

==========================================================================

	File: P_SiDec.h

	Description:

	Author: Jens Pommer, AU-System Radio AB

	Revision history:
  	Date    Sign	Comment
  	991115	JPR		First version
	000207	JPR		Updated to support new WBXML-decoder	
	010126	NKE		New definition of Si_AppSpecParseMod

=========================================================================*/

/* Private include statements */

#include "cmmnrsrc.h"
#include "wbxmldec.h"
#include "wmlparse.h"
#include "hdrutil.h"
#include "wbxmlelm.h"
#include "pushdef.h"
#include "pushelm.h"


/*========================================================================

	Implemented function pointers for SI

=========================================================================*/

/*========================================================================
	Si_AppSpecParseMod
==========================================================================*/
void Si_AppSpecParseMod (pDECSTR pDecStr);


/*========================================================================
	Si_StoreAttributeValue
==========================================================================*/
BOOL Si_StoreAttributeValue (pDECSTR pDecStr, pELEMENTTYPE pElement,
							 UINT16 iAttribute, pELEMENTTYPE* ppAttrVal);


/*========================================================================
	Si_GetAttributeValue
==========================================================================*/
WCHAR* Si_GetAttributeValue (UINT16 iToken);


/*========================================================================
	Si_LiteralAttributeToToken
==========================================================================*/
UINT16 Si_LiteralAttributeToToken (WCHAR* pwchToken);


/*========================================================================
	Si_LiteralTagToToken
==========================================================================*/
UINT16 Si_LiteralTagToToken (WCHAR* pwchToken);


/*========================================================================
	Si_CheckPublicID
==========================================================================*/
BOOL Si_CheckPublicID (pDECSTR pDecStr, WCHAR* pwchPId, UINT8* piPId);

#endif
