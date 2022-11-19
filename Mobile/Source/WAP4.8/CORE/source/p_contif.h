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

	WAP Implementation Project

==========================================================================

	File: P_ContIF.h

	Description: PUSH Content Interface file.

	Author: Jens Pommer, AU-System Radio AB

	Revision history:
  	Date    Sign	Comment
  	991115	JPR		First version
	991116	JPR		Minor changes
	000207	JPR		Updated to support new WBXML-decoder
	000208	JPR		New functions added

=========================================================================*/

#ifndef _PUSHCONTENTIFH
#define _PUSHCONTENTIFH

#include "p_sidec.h"
#include "p_sldec.h"
#include "p_codec.h"
#include "Push.h"

/*========================================================================

	Service Indication Functions

=========================================================================*/

/*========================================================================
	Si_BuildStructure
==========================================================================
Purpose:  Function to build the element structure needed to perform 
		  different operations. The returned value is the structure, or 
		  NULL if error. The pbData is left unchanged. It is the caller's 
		  responsibility to delete the structure after it is used. The 
		  parameters are as follows: 

		  pbData - pointer to the first byte in the indata.
		  pbEnd - pointer to the byte AFTER the last byte in the indata.
		  iCharset - integer containing the IANA-code of the charset.
		  iContentLevel - The level (WSP parameter) of the contenttype.
		  iViewID - The view for the user agent (used for error output)
======================================================================== */
pDECSTR Si_BuildStructure (BYTE *pbData, BYTE *pbEnd, INT16 iCharset, 
						   UINT8 iContentLevel, UINT8 iViewID);

/*========================================================================
	Si_DeleteStructure
==========================================================================
Purpose:  Function to delete a Si structure. The input (**) element 
		  must be of the type Si.
======================================================================== */
void Si_DeleteStructure (pDECSTR* ppDecStr);

/*========================================================================
	Si_GetSi
==========================================================================
Purpose:  Function to get the Si element of an Si structure. 
======================================================================== */
pELEMENTTYPE Si_GetSi (pDECSTR pDecStr);

/*========================================================================
	Si_GetIndication
==========================================================================
Purpose:  Function to get the Indication element of an Si structure. 
		  The input element must be of the type Si structure.
======================================================================== */
pELEMENTTYPE Si_GetIndication (pELEMENTTYPE pSi);

/*========================================================================
	Si_GetInfo
==========================================================================
Purpose:  Function to get the Info element of an Si structure. 
		  The input element must be of the type Si structure.
======================================================================== */
pELEMENTTYPE Si_GetInfo (pELEMENTTYPE pSi);

/*========================================================================
	Si_GetHref
==========================================================================
Purpose:  The function returns a copy of the href in the Si. 
		  NULL is returned if no href is present. The input element must 
		  be of the type Si Indication element. The string MUST be deleted
		  after it is used.
======================================================================== */
BYTE *Si_GetHref (pELEMENTTYPE pIndication);

/*========================================================================
	Si_GetIndicationText
==========================================================================
Purpose:  The function returns a copy of the body text in the Si. 
		  NULL is returned if no text is present. The input element must 
		  be of the type Si Indication element. The string MUST be deleted
		  after it is used.
======================================================================== */
WCHAR *Si_GetIndicationText (pDECSTR pDecStr, pELEMENTTYPE pIndication);

/*========================================================================
	Si_GetSiId
==========================================================================
Purpose:  The function returns a copy of the si-id in the Si. 
		  NULL is returned if no si-id is present. The input element must 
		  be of the type Si Indication element. The string MUST be deleted
		  after it is used.
======================================================================== */
WCHAR *Si_GetSiId (pELEMENTTYPE pIndication);

/*========================================================================
	Si_GetCreated
==========================================================================
Purpose:  The function returns a copy of the Created date in the Si. 
		  0 is returned if no date is present. The input element must 
		  be of the type Si Indication element. 
======================================================================== */
UINT32 Si_GetCreated (pELEMENTTYPE pIndication);

/*========================================================================
	Si_GetExpires
==========================================================================
Purpose:  The function returns a copy of the Expires date in the Si. 
		  0 is returned if no date is present. The input element must 
		  be of the type Si Indication element. 
======================================================================== */
UINT32 Si_GetExpires (pELEMENTTYPE pIndication);

/*========================================================================
	Si_GetAction
==========================================================================
Purpose:  The function returns a copy of the Action data in the Si. The 
          return values are defined in PUSHDef.h
		  Default value (signal-medium) is returned if no data is present. 
		  The input element must be of the type Si Indication element. 
======================================================================== */
UINT8 Si_GetAction (pELEMENTTYPE pIndication);

/*========================================================================
	Si_GetNextItem
==========================================================================
Purpose:  The function returns the item in the Si. 
		  NULL is returned if no item or no more items is/are present. The 
		  input element must be of the type Si Info element. If pItem
		  is NULL, the first item element is retrieved. 
======================================================================== */
pELEMENTTYPE Si_GetNextItem (pELEMENTTYPE pInfo, pELEMENTTYPE pItem);

/*========================================================================
	Si_GetClass
==========================================================================
Purpose:  The function returns a copy of the class data in the Si. 
		  NULL is returned if no class is present. The input element must 
		  be of the type Si Info element. The string MUST be deleted
		  after it is used.
======================================================================== */
WCHAR *Si_GetClass (pELEMENTTYPE pItem);

/*========================================================================
	Si_GetItemText
==========================================================================
Purpose:  The function returns a copy of the class text in the Si. 
		  NULL is returned if no text is present. The input element must 
		  be of the type Si Info element. The string MUST be deleted
		  after it is used.
======================================================================== */
WCHAR *Si_GetItemText (pDECSTR pDecStr, pELEMENTTYPE pItem);






/*========================================================================

	Service Loading Functions

=========================================================================*/

/*========================================================================
	Sl_BuildStructure
==========================================================================
Purpose:  Function to build the element structure needed to perform 
		  different operations. The returned value is the structure, or 
		  NULL if error. The pbData is left unchanged. It is the caller's 
		  responsibility to delete the structure after it is used. The 
		  parameters are as follows: 

		  pbData - pointer to the first byte in the indata.
		  pbEnd - pointer to the byte AFTER the last byte in the indata.
		  iCharset - integer containing the IANA-code of the charset.
		  iContentLevel - The level (WSP parameter) of the contenttype.
		  iViewID - The view for the user agent (used for error output)
======================================================================== */
pDECSTR Sl_BuildStructure (BYTE *pbData, BYTE *pbEnd, INT16 iCharset,
						   UINT8 iContentLevel, UINT8 iViewID);

/*========================================================================
	Sl_DeleteStructure
==========================================================================
Purpose:  Function to delete a Sl structure. The input (**) element 
		  must be of the type Sl.
======================================================================== */
void Sl_DeleteStructure (pDECSTR *ppDecStr);

/*========================================================================
	Sl_GetSl
==========================================================================
Purpose:  Function to get the Sl element of an Sl structure. 
======================================================================== */
pELEMENTTYPE Sl_GetSl (pDECSTR pDecStr);

/*========================================================================
	Sl_GetHref
==========================================================================
Purpose:  The function returns a copy of the href in the Sl. 
		  NULL is returned if no href is present. The input element must 
		  be of the type Sl. The string MUST be deleted after it is used.
======================================================================== */
BYTE *Sl_GetHref (pELEMENTTYPE pSl);

/*========================================================================
	Sl_GetAction
==========================================================================
Purpose:  The function returns a copy of the Action data in the Sl. The 
          return values are defined in PUSHDef.h
		  Default value (execute-low) is returned if no data is present. 
		  The input element must be of the type Sl. 
======================================================================== */
UINT8 Sl_GetAction (pELEMENTTYPE pSl);







/*========================================================================

	Cache Operation Functions

=========================================================================*/

/*========================================================================
	Co_BuildStructure
==========================================================================
Purpose:  Function to build the element structure needed to perform 
		  different operations. The returned value is the structure, or 
		  NULL if error. The pbData is left unchanged. It is the caller's 
		  responsibility to delete the structure after it is used. The 
		  parameters are as follows: 

		  pbData - pointer to the first byte in the indata.
		  pbEnd - pointer to the byte AFTER the last byte in the indata.
		  iCharset - integer containing the IANA-code of the charset.
		  iContentLevel - The level (WSP parameter) of the contenttype.
		  iViewID - The view for the user agent (used for error output)
======================================================================== */
pDECSTR Co_BuildStructure (BYTE *pbData, BYTE *pbEnd, INT16 iCharset, 
						   UINT8 iContentLevel, UINT8 iViewID);


/*========================================================================
	Co_DeleteStructure
==========================================================================
Purpose:  Function to delete a Co structure. The input (**) element 
		  must be of the type Co.
======================================================================== */
void Co_DeleteStructure (pDECSTR *ppDecStr);


/*========================================================================
	Co_GetCo
==========================================================================
Purpose:  Function to get the Co element of an Co structure. 
======================================================================== */
pELEMENTTYPE Co_GetCo (pDECSTR pDecStr);


/*========================================================================
	Co_GetInvalidateURI
==========================================================================
Purpose:  The function returns a copy of the URI in the Co. 
		  NULL is returned if no URI is present. The input element must 
		  be of the type Invalidate element. The string MUST be deleted 
		  after it is used.
======================================================================== */
BYTE *Co_GetInvalidateURI (pELEMENTTYPE pElement, void* vsHeaderDef);


/*========================================================================
	Co_GetInvalidateType
==========================================================================
Purpose:  The function returns a copy of the Invalidate type in the Co with
		  the following possible values: 
		  0: Unknown type (indicates error)
		  1: Invalidate Object
		  2: Invalidate Service
		  The input element must be of the type Invalidate element. 
======================================================================== */
UINT8 Co_GetInvalidateType (pELEMENTTYPE pElement);


/*========================================================================
	Co_GetNextInvElem
==========================================================================
Purpose:  The function returns the next Invalidate element in the Co. 
		  NULL is returned if no item or no more items is/are present. 
		  The input element must be of the type Co Invalidate element. If 
		  pElement is NULL, the first Invalidate element is retrieved. 
======================================================================== */
pELEMENTTYPE Co_GetNextInvElem (pELEMENTTYPE pCo,pELEMENTTYPE pElement);


#endif
