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
	FILE: wmlelm.h
  
	Interface file for WML-specific element handler.

	Rev history:
		980706	JPR		Created
		981223	JPR		Event concept changed	
		990303	JPR		Corrections		
		990316	JPR		Major changes. Handles the WML-specific 
						elements. Variable handling moved to 
						WMLIF.h
		990317	JPR		Corrections
		990323	JPR		WAP 1.1
		990412	JPR		IsAlpha and IsNumber functions removed
		990512	JPR		ViewID added to WML_CreateElement function.
		990827	JPR		WML_AddContentToEnd added
		991026	JPR		Additions for WTA included.

		991202	JPR		Changes to support new generic WBXML-decoder
		991208	JPR		Minor corrections
		000104	JPR		Function WML_SetXmlSpace added
		000201	JPR		Corrections in Set-functions for attributes
		000225	JPR		Function WML_SetXmlSpace updated

==========================================================================*/

#ifndef _ELMHANDLERH
#define _ELMHANDLERH

#include "wmldef.h"
#include "xml_conv.h"

/*========================================================================
	WML_AddTemplateDO
==========================================================================
	The function adds the DO element to the Template DO list on the card. 
	
	Note! Only the pointer are stored -  DO NOT DEALLOCATE the DO element.

	Input: Pointer to Card and the DO element (MUST be defined)
	Output: TRUE if success, FALSE otherwise
==========================================================================*/
BOOL WML_AddTemplateDO (pELEMENTTYPE pCard, pELEMENTTYPE pDO);


/*========================================================================
	WML_ConvertEventToInt, WML_ConvertInputToInt
==========================================================================
	The functions converts the input/event type to an integer. If the type 
	exists the function returns the appropriate number. TYPE_UNKNOWN is 
	returned if the string is not an intrinsic event or 'text' / 'password'

	Note! The inputted string is deallocated after the operation.

	Input: WCHAR**
	Output: INT8 according to TYPE code table.
==========================================================================*/
INT8 WML_ConvertEventToInt (WCHAR** ppchType);
INT8 WML_ConvertInputToInt (WCHAR** ppchType);


#ifdef CONFIG_WTA
/*========================================================================
	WTA_ConvertEventToInt
==========================================================================
	The function converts the WTA-event type to an integer. If the type 
	exists the function returns the appropriate number. TYPE_UNKNOWN is 
	returned if the string is not an WTA event.

	Note! The inputted string is NOT deallocated after the operation.

	Input: WCHAR**
	Output: INT8 according to TYPE code table.
==========================================================================*/
INT8 WTA_ConvertEventToInt (WCHAR *pwchType);
#endif

/*========================================================================
	WML_ConvertAlign
==========================================================================
	The function converts the align to an integer. If the type exists the 
	function returns the appropriate number. TYPE_UNKNOWN is returned if 
	the string is not known.

	Note! The inputted string is deallocated after the operation.

	Input: WCHAR**
	Output: INT8 according to TYPE code table.
==========================================================================*/
INT8 WML_ConvertAlign (WCHAR **ppchAlign);


/*========================================================================
	WML_ConvertAlign
==========================================================================
	The function converts the align to an integer. If the type exists the 
	function returns the appropriate number. TYPE_UNKNOWN is returned if 
	the string is not known.

	Note! The inputted string is deallocated after the operation.

	Input: WCHAR**
	Output: INT8 according to TYPE code table.
==========================================================================*/
INT8 WML_ConvertAlign (WCHAR **ppchAlign);



/*========================================================================

	Implemented function pointers for WML

=========================================================================*/

/*========================================================================
	WML_CreateElement
==========================================================================
	WML-specific implementation of DTD_CreateElementFunc.
==========================================================================*/
pELEMENTTYPE WML_CreateElement (pDECSTR pDecStr, UINT16 iType);


/*========================================================================
	WML_DeleteElement
==========================================================================
	WML-specific implementation of DTD_DeleteElementFunc.
==========================================================================*/
void WML_DeleteElement (pDECSTR pDecStr, pELEMENTTYPE* ppElement);


/*========================================================================
	WML_ValidContent
==========================================================================
	WML-specific implementation of DTD_ValidContentFunc.
==========================================================================*/
BOOL WML_ValidContent (pDECSTR pDecStr, pELEMENTTYPE pElement); 


/*========================================================================
	WML_GetContent
==========================================================================
	WML-specific implementation of DTD_GetContentFunc.
==========================================================================*/
pELEMENTTYPE WML_GetContent (pELEMENTTYPE pElement);




/*========================================================================

	Attribute functions

=========================================================================*/

/*========================================================================
	WML_Set...
==========================================================================
	The following functions sets the attribute values in the elements. If
	the attribute field and the attribute value is valid for the element 
	it is and stored and TRUE is returned. Otherwise, FALSE is returned. 

	NOTE! It is the callers responsibility to deallocate all parts NOT 
	used by the functions. For example, if ppAlign is not used in the 
	function WML_SetAlign, this memory must be deallocated.
	
	Input: Element (NOT NULL) and attribute value (NOT NULL).						
	Output: BOOL
==========================================================================*/
BOOL WML_SetAccChar		(pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppAccChar);
BOOL WML_SetAccesskey	(pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppAccesskey);
BOOL WML_SetAlign		(pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppAlign, INT8 iAlign);
BOOL WML_SetAlt			(pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppAlt);
BOOL WML_SetCacheControl (pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppCacheControl, enum CacheControl iCache);
BOOL WML_SetColumns		(pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppColumns);
BOOL WML_SetContent		(pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppContent);
BOOL WML_SetDomain		(pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppDomain);
BOOL WML_SetEmptyOK		(pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppEmptyOK, BOOL fEmptyOK);
BOOL WML_SetEnctype		(pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppEnctype);
BOOL WML_SetFormat		(pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppFormat);
BOOL WML_SetForua		(pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppForua, BOOL fForua);
BOOL WML_SetHeight		(pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppHeight);
BOOL WML_SetHSpace		(pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppHSpace);
BOOL WML_SetHref		(pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppHref);
BOOL WML_SetHttpEq		(pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppHttpEq);
BOOL WML_SetId			(pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppId);
BOOL WML_SetIValue		(pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppIValue);
BOOL WML_SetIName		(pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppIName);
BOOL WML_SetLabel		(pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppLabel);
BOOL WML_SetLocalSrc	(pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppLocalSrc);
BOOL WML_SetMaxLength	(pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppLength);
BOOL WML_SetMethod		(pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppMethod, enum MethodType iMethod);
BOOL WML_SetMode		(pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppMode, enum WrapMode iMode);
BOOL WML_SetMultiple	(pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppMultiple, BOOL fMultiple);
BOOL WML_SetName		(pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppName);
BOOL WML_SetNewContext	(pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppNewContext, BOOL fContext);

BOOL WML_SetOnEntBack	(pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppURL);
BOOL WML_SetOnEntForw	(pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppURL);
BOOL WML_SetOnPick		(pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppURL);
BOOL WML_SetOnTimer		(pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppURL);

BOOL WML_SetOptional	(pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppOptional, BOOL fOptional);
BOOL WML_SetPath		(pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppPath);
BOOL WML_SetScheme		(pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppScheme);
BOOL WML_SetSendReferer (pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppSendRef, BOOL fSendRef);
BOOL WML_SetSize		(pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppSize);
BOOL WML_SetSrc			(pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppHref);
BOOL WML_SetOrdered		(pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppOrdered, BOOL fOrdered);
BOOL WML_SetTabIndex	(pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppTabIndex);
BOOL WML_SetTitle		(pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppTitle);
BOOL WML_SetType		(pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppType, BOOL fWTA);
BOOL WML_SetValue		(pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppValue);
BOOL WML_SetVSpace		(pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppVSpace);
BOOL WML_SetWidth		(pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppWidth);
BOOL WML_SetXmllang		(pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppXmllang);
BOOL WML_SetXmlSpace	(pDECSTR pDecStr, pELEMENTTYPE pElement, pELEMENTTYPE* ppXmlSpace, UINT8 iSpace);

#endif
